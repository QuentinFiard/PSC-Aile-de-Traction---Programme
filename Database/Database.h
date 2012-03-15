//
//  Database.h
//  PSC
//
//  Created by Quentin Fiard on 11/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Database_h
#define PSC_Database_h

#include <string>
#include <vector>
#include <utility>
#include <sqlite3.h>

#include <iostream>
#include <assert.h>

template<typename T>
class Source;
template<typename T>
class Donnee;

class Record;

class GenericSource;

#include "DatabaseData.h"

class Database
{
public:
	
#pragma mark - Fonctions utiles
	
	static std::vector<uint8_t> toVector(DatabaseData& value);
	static DatabaseData* fromVector(std::vector<uint8_t>& value,DataType type);
	
#pragma mark - Configuration Field
	
	static void setConfigurationField(std::string field, DatabaseData* value);
	static DatabaseData* getConfigurationField(std::string field);
	
	template<typename T>
	static T getConfigurationFieldValue(std::string field);
	
	static std::string changeConfigurationFieldName(std::string oldFieldName, std::string newFieldName);
	
	static std::vector< std::pair<std::string,DataType> > getAllConfigurationFields();
	
	static void removeConfigurationField(std::string field);
	
	static int indexOfField(std::string field);
	
#pragma mark - Records
	
	static std::vector<Record*> getAllRecords();
	static void saveRecord(Record* record);
	static void removeRecord(Record* record);
	
#pragma mark - Source
	
	template<typename T>
	static void saveSource(Source<T>& source);
	static std::vector<GenericSource*> getAllSourcesForRecord(Record* record);
	static void removeSource(GenericSource* source);
	
#pragma mark - Donnee
	
	template<typename T>
	static void saveData(Donnee<T>& donnee);
	template<typename T>
	static bool findData(Donnee<T>& donnee);
	
	
	
private:
	Database();
	~Database();
	
	sqlite3* database;
	
	static Database& shared();
	
	
	void setConfigurationField_(std::string& field, DatabaseData* data);
	DatabaseData* getConfigurationField_(std::string& field);
	std::vector< std::pair<std::string,DataType> > getAllConfigurationFields_();
	void removeConfigurationField_(std::string field);
	int indexOfField_(std::string field);
	
	template<typename T>
	bool findData_(Donnee<T>& donnee);
	template<typename T,typename S>
	void saveDataWithValue_(T& value, Donnee<S>& donnee);
	template<typename T>
	void saveDataWithValue_(std::vector<uint8_t> value, Donnee<T>& donnee);
	
	template<typename T>
	void saveSource_(Source<T>& source);
	std::vector<GenericSource*> getAllSourcesForRecord_(Record* record);
	void removeSource_(GenericSource* source);
	
	std::vector<Record*> getAllRecords_();
	void saveRecord_(Record* record);
	void removeRecord_(Record* record);
	
	std::string changeConfigurationFieldName_(std::string oldFieldName, std::string newFieldName);

};

#pragma mark - Implementation

#include "DatabaseData.h"

#include <boost/date_time.hpp>

#include "Donnee.h"
#include "Source.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

template<typename T,typename S>
void Database::saveDataWithValue_(T& value, Donnee<S>& donnee)
{
	saveDataWithValue_(Database::toVector(value),donnee);
}

template<typename T>
void Database::saveDataWithValue_(std::vector<uint8_t> value, Donnee<T>& donnee)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	if(findData_(donnee))
	{
		cmd = "UPDATE Source SET value=?001 WHERE ROWID=?002";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_blob(statement, 1, value.data(), value.size(), SQLITE_STATIC);
		sqlite3_bind_int64(statement, 2, donnee.ID());
		
		sqlite3_step(statement);
	}
	else
	{
		cmd = "INSERT INTO Source(date,sourceID,value) VALUES (?001,?002,?003)";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		ptime ref(date(2001,Jan,1));
		
		boost::int64_t offset = (donnee.date()-ref).total_nanoseconds();
		
		sqlite3_bind_int64(statement, 1, offset);
		sqlite3_bind_int64(statement, 2, donnee.source().ID());
		sqlite3_bind_blob(statement, 3, value.data(), value.size(), SQLITE_STATIC);
		
		sqlite3_step(statement);
		
		donnee.setID(sqlite3_last_insert_rowid(database));
	}
	
	sqlite3_finalize(statement);
}

template<typename T>
void Database::saveData(Donnee<T>& donnee)
{
	Database::shared().saveDataWithValue_(donnee.value(),donnee);
}

template<typename T>
bool Database::findData(Donnee<T>& donnee)
{
	return Database::shared().findData_(donnee);
}

template<typename T>
bool Database::findData_(Donnee<T>& donnee)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT ROWID FROM Donnee WHERE date=?001 AND sourceID=?002";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	ptime ref(date(2001,Jan,1));
	
	boost::int64_t offset = (donnee.date()-ref).ticks();
	
	sqlite3_bind_int64(statement, 1, offset);
	sqlite3_bind_int64(statement, 2, donnee.source().ID());
	
	bool res = (sqlite3_step(statement) == SQLITE_ROW);
	
	if(res)
	{
		donnee.setID(sqlite3_column_int64(statement,0));
	}
	else
	{
		donnee.clearID();
	}
	
	sqlite3_finalize(statement);
	
	return res;
}

#pragma mark - Source

template<typename T>
void Database::saveSource(Source<T>& source)
{
	Database::shared().saveSource_(source);
}

template<typename T>
void Database::saveSource_(Source<T>& source)
{
	sqlite_int64 sourceId = source.ID();
	
	bool sourceStillExists = false;
	sqlite3_stmt* statement;
	std::string cmd;
	
	if(sourceId != -1)
	{
		cmd = "SELECT ROWID FROM Source WHERE ROWID=?001";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_int64(statement, 1, sourceId);
		
		sourceStillExists = (sqlite3_step(statement) == SQLITE_ROW);
		
		sqlite3_finalize(statement);
	}
	
	if(sourceStillExists)
	{
		cmd = "UPDATE Source SET name=?001,dataType=?002 WHERE ROWID=?003";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_text(statement, 1, source.name().c_str(), source.name().length(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 2, source.type());
		sqlite3_bind_int64(statement, 3, sourceId);
		
		sqlite3_step(statement);
	}
	else
	{
		cmd = "INSERT INTO Source(name,dataType) VALUES (?001,?002)";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_text(statement, 1, source.name().c_str(), source.name().length(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 2, source.type());
		
		sqlite3_step(statement);
		
		source.setID(sqlite3_last_insert_rowid(database));
	}
	
	sqlite3_finalize(statement);
}

template<typename T>
static T Database::getConfigurationFieldValue(std::string field)
{
	std::cout << "Type non supporté";
	assert(0);
	return T();
}

#endif
