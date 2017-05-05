#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <fstream>
#include <sstream>
#include <string>
#include <ctime> 
#include <dirent.h>

using namespace std;

/// Exception
/**
* Class for exceptions when there is some error.
* It stores Name of an error.
*/
class CException
{
    public:
	/// Constructor
	/**
	* \param x name of an error
	*/
	CException(const string& name);
	/// Friend function for writing an error into stream
	/**
	* \param x Stream in which an error will be written \param y Error
	*/
	friend ostream& operator<<(ostream& os,const CException& exception);

   private:
	string m_name;

};

/// Config
/**
* Class for getting configuration data for server.
* It stores path to root, path to log, IP address and port
*/
class CConfig
{
    public:   
	/// Constructor
	CConfig();
	/// Destructor
	~CConfig();
	/// Reading configuration data from a configuration file
	void getConfig();
	string getRoot() const;
	string getPort() const;
	string getLog() const;
	string getIP() const;

    private:
	string m_root;
	string m_port;
	string m_log;
	string m_ip;
	static const string CONFIGERROR;

};

/// Log
/**
* Class for writing logs into a log file.
* It stores client's IP address.
*/
class CLog
{
    public:
	/// Constructor
	/**
	* \param ip client's IP address
	*/
	CLog( const string & ip );
	/// Destructor
	~CLog();
	/// Writing log into a log file
	/**
	* \param time Time \param request Request from a client
	* \param response Code of the response \param len Length of the response
	* \param where Path to the log file
	*/
	void getLog( const string& time, const string& request, int response, int len, const string& where ) const;

    private:
	string m_ip;
	static const string FILEERROR;

};

/// Time
/**
* Class for getting time in a particular format.
* It stores a pointer on a struct tm which is time structure.
*/
class CTime
{
    public:
	/// Constructor
	/**
	* \param timeinfo Timeinfo it is pointer to structure which stores current time
	*/
	CTime( struct tm* timeinfo );
	/// Getting current time from struct tm
	/**
	* \return Time in a particular format
	*/
	/// Destructor
	~CTime();
	string getTime( ) const;

    private:
	/// Auxiliary method for getting Day from an integer number in range of 0-6
	/**
	* \param num Number of the day
	* \return Name of the day
	*/
	string intToDay( int num ) const;
	/// Auxiliary method for gettinf Month from and integer number in range of 0-11
	/**
	* \param num Number of the month 
	* \return Name of the month
	*/
	string intToMonth( int num ) const;
	/// Auxiliary method for getting string which is transformed from integer and also from numbers 0-9 it will make 00-09
	/**
	* \param num Number
	* \return Number in string form but in format "00-09"
	*/
	string intToZero( int num ) const;

	struct tm* m_timeinfo;

};


#endif /* AUXILIARY_H */

