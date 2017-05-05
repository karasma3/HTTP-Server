#include "polymorph.h"

//---------CREQUEST-------------

const string CRequest::NOTFOUND = "Not Found";

const string CRequest::BADREQUEST = "Error bad request";

CRequest::CRequest()
{

}

CRequest::~CRequest()
{

}

//----------CPAGE-------------

CPage::CPage( const string& head, const string& clientIP )
{
	m_head = head;
	m_clientIP = clientIP;

}

CPage::~CPage()
{

}

void CPage::Response( int clientSock, const CConfig& config, const CTime& b ) const
{
	bool flag;
	int response = OK;
	int content_length = 0;
	vector<string> tmp_vec;
	string request, page, protocol, path, type;
	stringstream stream;

	stream.str( m_head );
	getline( stream, request, ' ' );
	getline( stream, page, ' ' );
	getline( stream, protocol );

	if( request != "GET" && request != "HEAD" )
	{
		string line = BADREQUEST + "\r\n";
		char* buffer = new char[line.length()];
		int len = 0;
		for( unsigned i = 0; i < line.length(); i++ )
		{
			buffer[i] = line[i];
			len++;
		}
		send ( clientSock, buffer, len, 0 );
		delete [] buffer;

		throw CException( BADREQUEST );
	}

	stream.str();
	stream.clear();
	stream.str( page );
	getline( stream, path, '.' );
	getline( stream, type ); 

	ifstream f( config.getRoot() + page );

	if( f. fail() || ( request != "GET" && request != "HEAD" ) )
	{
		flag = false;
	}
	else
	{
		while( !f.eof() )
		{
			string line;
			getline( f, line );
			tmp_vec.push_back( line );
			line += '\n';
			content_length += line.length();
		}		

		content_length--;
		flag = true;
	}

	string content_type;

	if( type == "html" || type == "css" )
	{
		content_type = "text/" + type;
	}
	else if( type == "png" || type == "jpg" || type == "jpeg" || type == "gif" || type == "bmp" )
	{
		content_type = "image/" + type;
	}
	else if( type == "htm" )
	{
		content_type = "text/html";
	}
	else if( type == "js" )
	{
		content_type = "text/javascript";
	}
	else if( type == "ico" )
	{
		content_type = "image/vnd.microsoft.icon";
	}
	else if( type == "svg" )
	{
		content_type = "image/svg+xml";
	}
	else if( type == "txt" )
	{
		content_type = "text/plain";
	}
	if( !flag )
	{
		int len = 0;
		response = ERROR;
		string line, msg;

		msg = "Error " + to_string( response ) + " " + NOTFOUND + "\r\n\r\n";
		content_length = msg.length();

		line = protocol + " " + to_string( response ) + " " + NOTFOUND + "\r\n";
		line += "Date: " +  b.getTime() + "\r\nContent-Length: " + to_string( content_length );
		line += "\r\nContent-Type: " + content_type+"\r\n\r\n";
		line += msg;

		char* buffer = new char[line.length()];

		for( unsigned i = 0; i < line.length(); i++ )
		{
			buffer[i] = line[i];
			len++;
		}

		send ( clientSock, buffer, len, 0 );

		delete [] buffer;
	}
	else
	{
		int len = 0;
		string line;

		line = protocol + " " + to_string( response ) + " OK\r\n" + "Date: ";
		line +=  b.getTime() + "\r\nContent-Length: " + to_string( content_length );
		line += "\r\nContent-Type: " + content_type+"\r\n\r\n";

		char* buffer = new char[line.length()];

		for( unsigned i = 0; i < line.length(); i++ )
		{
			buffer[i] = line[i];
			len++;
		}

		send ( clientSock, buffer, len, 0 );

		delete [] buffer;

		if( request == "HEAD" )
		{
			return;
		}

		for( auto i = tmp_vec.begin(); i != tmp_vec.end(); ++i )
		{
			char* buffer = new char[(*i).length()+1];
			int len = 0;

			for( unsigned j = 0; j < (*i).length(); j++ )
			{
				buffer[j] = (*i)[j];
				len++;
			}

			buffer[len] = '\n';
			len++;

			send ( clientSock, buffer, len, 0 );

			delete [] buffer;
		}
	}

	f.close();

	CLog log( m_clientIP );
	try
	{
		log.getLog( b.getTime(), m_head, response, content_length, config.getRoot() + config.getLog() );
	}
	catch ( const CException& exception )
	{
		cout << exception;
	}

}

//------------CCONTENT-------------

CContent::CContent( const string& head, const string& clientIP )
{
	m_head = head;
	m_clientIP = clientIP;

}

CContent::~CContent()
{

}

void CContent::Response( int clientSock,  const CConfig& config, const CTime& b ) const
{
	int response = OK;
	int content_length = 0;
	string request, content, protocol;
	stringstream stream;
	set<string> tmp_set;

	stream.str( m_head );
	getline( stream, request, ' ' );
	getline( stream, content, ' ' );
	getline( stream, protocol );

	if( request != "GET" && request != "HEAD" )
	{
		string line = BADREQUEST + "\r\n";
		char* buffer = new char[line.length()];
		int len = 0;
		for( unsigned i = 0; i < line.length(); i++ )
		{
			buffer[i] = line[i];
			len++;
		}
		send ( clientSock, buffer, len, 0 );
		delete [] buffer;

		throw CException( BADREQUEST );
	}

	string where = config.getRoot() + content;

	DIR *pdir = NULL;
	pdir = opendir ( where.c_str() );

	if( pdir )
	{
		struct dirent *pent = NULL;
		while ( 1 )
		{
			pent = readdir( pdir );
			if( !pent )
			{
				break;
			}
			string str( pent -> d_name );
			tmp_set.insert( str );
			if( str == "." || str == ".." )
			{
				continue;
			}
			content_length += str.length()+2;
		}
		closedir( pdir );

		string line;
		int len = 0;
		line = protocol + " " + to_string( response ) + " OK\r\n" + "Date: " +  b.getTime();
		line += "\r\nContent-Length: " + to_string( content_length ) + "\r\nContent-Type: text/plain\r\n\r\n";

		char* buffer = new char[line.length()];

		for( unsigned i = 0; i < line.length(); i++ )
		{
			buffer[i] = line[i];
			len++;
		}

		send ( clientSock, buffer, len, 0 );

		delete [] buffer;

		if( request == "HEAD" )
		{
			return;
		}

		for( auto i=tmp_set.begin(); i != tmp_set.end(); ++i )
		{
			if( (*i) == "." || (*i) == ".." )
			{
				continue;
			}
			int len = 0;
			char* buffer = new char[(*i).length()+2];

			for( unsigned j = 0; j < (*i).length(); ++j )
			{
				buffer[j] = (*i)[j];
				len++;
			}

			buffer[len] = '\r';
			len++;
			buffer[len] = '\n';
			len++;

			send ( clientSock, buffer, len, 0 );

			delete [] buffer;
		}
	}
	else
	{
		response = ERROR;
		string line, msg;
		int len = 0;

		msg = "Error " + to_string( response ) + " " + NOTFOUND + "\r\n\r\n";
		content_length = msg.length();

		line = protocol + " " + to_string( response ) +" " + NOTFOUND + "\r\n" + "Date: " +  b.getTime();
		line += "\r\nContent-Length: " + to_string( content_length ) + "\r\nContent-Type: text/html\r\n\r\n";
		line += msg;

		char* buffer = new char[line.length()];

		for( unsigned i = 0; i < line.length(); i++ )
		{
			buffer[i] = line[i];
			len++;
		}

		send ( clientSock, buffer, len, 0 );

		delete [] buffer;
	}

	CLog log( m_clientIP );
	try
	{
		log.getLog( b.getTime(), m_head, response, content_length, config.getRoot() + config.getLog() );
	}
	catch ( const CException& exception )
	{
		cout << exception;
	}

}

