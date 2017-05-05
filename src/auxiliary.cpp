#include "auxiliary.h"

//----------CEXCEPTION----------

CException::CException(const string& name):m_name(name)
{

}

ostream& operator<<(ostream& os,const CException& exception)
{
	os<<exception.m_name<<"."<<endl; 

	return os;

}

//-----------CCONFIG----------

const string CConfig::CONFIGERROR = "Error bad config file";

CConfig::CConfig()
{

}

CConfig::~CConfig()
{

}

void CConfig::getConfig()
{
	string inFile = "src/config.txt";

	ifstream f( inFile.c_str(), ios::in );

	if( f.fail() )
	{
		throw CException( CONFIGERROR );
	}

	string line;

	while( getline( f, line ) )
	{
		stringstream stream;
		stream.str( line );
		string what;
		string where;
		getline( stream, what, '=' );
		getline( stream, where );

		if( where.length() == 0 )
		{
			throw CException( CONFIGERROR );
		}

		if( what == "root" )
		{
			m_root = where;
			where += "/";
			DIR *pdir = NULL;
			pdir = opendir ( where.c_str() );
			if( !pdir ) throw CException( CONFIGERROR );
			closedir( pdir );
		}
		else if( what == "port" )
		{
			m_port = where;
		}
		else if( what == "log" )
		{
			m_log = where;
			ofstream outFile( m_root + where, ios::out | ios::app );
			if( outFile.fail() )
			{
				throw CException( CONFIGERROR );
			}
			outFile.close();
		}
		else if( what == "ip" )
		{
			m_ip = where;
		}
		else
		{
			continue;
		}
   	}

	f.close();
}

string CConfig::getRoot() const
{
	return m_root;

}

string CConfig::getPort() const
{
	return m_port;

}

string CConfig::getLog() const
{
	return m_log;

}

string CConfig::getIP() const
{
	return m_ip;

}



//-----------------CLOG-----------

const string CLog::FILEERROR = "Error with opening log file";

CLog::CLog( const string& ip ):m_ip( ip )
{

}

CLog::~CLog()
{

}

void CLog::getLog( const string& time, const string& request, int response, int len, const string& where ) const
{
	ofstream outFile( where, ios::out | ios::app );
	if( outFile.fail() )
	{
		throw CException( FILEERROR );
	}
	outFile<<m_ip<<" ["<<time<<"] \""<<request<<"\" "<<response<<" "<<len<<endl;

	outFile.close();

}

//-----------------CTIME-------------

CTime::CTime( struct tm* timeinfo ):m_timeinfo( timeinfo )
{

}

CTime::~CTime()
{

}

string CTime::getTime( ) const
{
	string tmp;

	tmp = intToDay( m_timeinfo -> tm_wday ) + ", " + intToZero( m_timeinfo -> tm_mday );
	tmp += " " + intToMonth( m_timeinfo -> tm_mon ) + " " + to_string( m_timeinfo -> tm_year + 1900 );
	tmp += " " + intToZero( m_timeinfo -> tm_hour ) + ":" + intToZero( m_timeinfo -> tm_min );
	tmp += ":" + intToZero( m_timeinfo -> tm_sec ) + " GMT";

	return tmp;

}

string CTime::intToDay( int num ) const
{
	char allDays[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	string day( allDays[num] );
	return day;

}

string CTime::intToMonth( int num ) const
{
	char allMonths[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug","Sep", "Oct", "Nov", "Dec"};

	string month( allMonths[num] );
	return month;

}

string CTime::intToZero( int num ) const
{
	if( num > 9 )
	{
		string str = to_string( num );
		return str;
	}

	char allNums[10][3] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09"};

	string str( allNums[num] );
	return str;

}

