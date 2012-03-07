//
//  Database.cpp
//  PSC
//
//  Created by Quentin Fiard on 11/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Database.h"

#include "GenericSource.h"
#include "Record.h"

#include "Utilities.h"

#include <sqlite3.h>
#include <boost/date_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;


#pragma mark - Shared Database Object

static Database* shared_ = NULL;

#pragma mark - Fonctions utiles

std::vector<uint8_t> Database::toVector(DatabaseData& value)
{
	return value.toBinary();
}

DatabaseData* Database::fromVector(std::vector<uint8_t>& value,DataType type)
{
	return DatabaseData::dataWithType(value, type);
}

#pragma mark - Fonctions membres

Database::Database()
{
	sqlite3_open(pathToDatabase().c_str(), &database);
}

Database::~Database()
{
	sqlite3_close(database);
}

Database& Database::shared()
{
	if(!shared_)
	{
		shared_ = new Database();
	}
	return *shared_;
}

#pragma mark - Configuration

DatabaseData* Database::getConfigurationField(std::string field)
{
	return Database::shared().getConfigurationField_(field);
}

DatabaseData* Database::getConfigurationField_(std::string& field)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT value,dataType FROM Configuration WHERE field=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_text(statement, 1, field.c_str(), field.length(), SQLITE_STATIC);
	
	assert(sqlite3_step(statement) == SQLITE_ROW);
	
	uint8_t* buffer = (uint8_t*)sqlite3_column_blob(statement, 0);
	std::size_t length = sqlite3_column_bytes(statement, 0);
	
	std::vector<uint8_t> res(buffer,buffer+length);

	DataType type = sqlite3_column_int(statement, 1);
	
	sqlite3_finalize(statement);
	
	return DatabaseData::dataWithType(res, type);
}

std::vector< std::pair<std::string,DataType> > Database::getAllConfigurationFields()
{
	return Database::shared().getAllConfigurationFields_();
}

std::vector< std::pair<std::string,DataType> > Database::getAllConfigurationFields_()
{
	std::vector< std::pair<std::string,DataType> > res;
	
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT field,dataType FROM Configuration ORDER BY field ASC";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	while(sqlite3_step(statement) == SQLITE_ROW)
	{
		std::string name((const char*)sqlite3_column_text(statement,0));
		DataType type = sqlite3_column_int(statement, 1);
		std::pair<std::string,DataType> tmp(name,type);
		res.push_back(tmp);
	}
	
	sqlite3_finalize(statement);
	
	return res;
}

void Database::removeConfigurationField(std::string field)
{
	Database::shared().removeConfigurationField_(field);
}

void Database::removeConfigurationField_(std::string field)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "DELETE FROM Configuration WHERE field=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_text(statement, 1, field.c_str(), field.length(), SQLITE_STATIC);
	
	sqlite3_step(statement);
	
	sqlite3_finalize(statement);
}

std::string Database::changeConfigurationFieldName(std::string oldFieldName, std::string newFieldName)
{
	return Database::shared().changeConfigurationFieldName_(oldFieldName,newFieldName);
}

std::string Database::changeConfigurationFieldName_(std::string oldFieldName, std::string newFieldName)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT ROWID FROM Configuration WHERE field=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_text(statement, 1, newFieldName.c_str(), newFieldName.length(), SQLITE_STATIC);
	
	if(sqlite3_step(statement) != SQLITE_ROW)
	{
		sqlite3_finalize(statement);
		
		cmd = "SELECT ROWID FROM Configuration WHERE field=?001";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_text(statement, 1, oldFieldName.c_str(), oldFieldName.length(), SQLITE_STATIC);
		
		if(sqlite3_step(statement) == SQLITE_ROW)
		{
			sqlite_int64 rowID = sqlite3_column_int64(statement, 0);
			
			sqlite3_finalize(statement);
			
			cmd = "UPDATE Configuration SET field=?001 WHERE ROWID=?002";
			
			sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
			
			sqlite3_bind_text(statement, 1, newFieldName.c_str(), newFieldName.length(), SQLITE_STATIC);
			sqlite3_bind_int64(statement, 2, rowID);
			
			sqlite3_step(statement);
			sqlite3_finalize(statement);
			
			return newFieldName;
		}
		else
		{
			sqlite3_finalize(statement);
			
			return "";
		}
		
	}
	else
	{
		sqlite3_finalize(statement);
		
		return oldFieldName;
	}
}

int Database::indexOfField(std::string field)
{
	return Database::shared().indexOfField_(field);
}

int Database::indexOfField_(std::string field)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT count(ROWID) FROM Configuration WHERE field<?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_text(statement, 1, field.c_str(), field.length(), SQLITE_STATIC);
	
	sqlite3_step(statement);
	
	int res = sqlite3_column_int(statement,0);
	
	sqlite3_finalize(statement);
	
	return res;
}

bool Database::hasConfigurationField(std::string field)
{
	return Database::shared().hasConfigurationField_(field);
}

bool Database::hasConfigurationField_(std::string field)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT ROWID FROM Configuration WHERE field=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_text(statement, 1, field.c_str(), field.length(), SQLITE_STATIC);
	
	bool res = (sqlite3_step(statement)==SQLITE_ROW);
	
	sqlite3_finalize(statement);
	
	return res;
}


#pragma mark - Records

std::vector<Record*> Database::getAllRecords()
{
	return Database::shared().getAllRecords_();
}

std::vector<Record*> Database::getAllRecords_()
{
	std::vector<Record*> res;
	
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT ROWID,startDate,tag FROM Record ORDER BY startDate ASC";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	ptime ref(date(2001,Jan,1));
	
	while(sqlite3_step(statement) == SQLITE_ROW)
	{
		sqlite_int64 ID = sqlite3_column_int64(statement, 0);
		boost::int64_t offset = sqlite3_column_int64(statement,1);
		std::string tag((const char*)sqlite3_column_text(statement,2));
		
		time_duration offsetTime = microseconds(offset);
		
		ptime date(ref);
		date+=offsetTime;
		
		Record* tmp = new Record(ID,tag,date);
		
		res.push_back(tmp);
	}
	
	sqlite3_finalize(statement);
	
	return res;
}

void Database::saveRecord(Record* record)
{
	Database::shared().saveRecord_(record);
}

void Database::saveRecord_(Record* record)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT ROWID FROM Record WHERE startDate=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, record->date());
	
	if(sqlite3_step(statement) == SQLITE_ROW)
	{
		sqlite_int64 ID = sqlite3_column_int64(statement, 0);
		
		sqlite3_finalize(statement);
		
		cmd = "UPDATE Record SET tag=?001 WHERE ROWID=?002";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_text(statement, 1, record->tag().c_str(), record->tag().length(), SQLITE_STATIC);
		sqlite3_bind_int64(statement, 2, ID);
		
		sqlite3_step(statement);
	}
	else
	{
		sqlite3_finalize(statement);
		
		cmd = "INSERT INTO Record(startDate,tag) VALUES (?001,?002)";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_int64(statement, 1, record->date());
		sqlite3_bind_text(statement, 2, record->tag().c_str(), record->tag().length(), SQLITE_STATIC);
		
		sqlite3_step(statement);
		
		record->setID(sqlite3_last_insert_rowid(database));
	}
	
	sqlite3_finalize(statement);
}

void Database::removeRecord(Record* record)
{
	Database::shared().removeRecord_(record);
}

void Database::removeRecord_(Record* record)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "DELETE FROM Record WHERE ROWID=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, record->ID());
	
	sqlite3_step(statement);
	
	sqlite3_finalize(statement);
	
	cmd = "DELETE FROM Source WHERE recordId=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, record->ID());
	
	sqlite3_step(statement);
	
	sqlite3_finalize(statement);
}

#pragma mark - Source

std::vector<GenericSource*> Database::getAllSourcesForRecord(Record* record)
{
	return Database::shared().getAllSourcesForRecord_(record);
}

std::vector<GenericSource*> Database::getAllSourcesForRecord_(Record* record)
{
	std::vector<GenericSource*> res;
	
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT ROWID,name,dataType FROM Source WHERE recordId=?001 ORDER BY name ASC";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite_int64 recordId = record->ID();
	
	sqlite3_bind_int64(statement, 1, recordId);
	
	while(sqlite3_step(statement) == SQLITE_ROW)
	{
		sqlite_int64 ID = sqlite3_column_int64(statement, 0);
		std::string name((const char*)sqlite3_column_text(statement, 1),sqlite3_column_bytes(statement, 1));
		DataType type = sqlite3_column_int(statement, 2);
		
		res.push_back(GenericSource::sourceWithIDNameTypeAndRecordID(ID, name, type, record));
	}
	
	sqlite3_finalize(statement);
	
	return res;
}

void Database::removeSource(GenericSource* source)
{
	Database::shared().removeSource_(source);
}

void Database::removeSource_(GenericSource* source)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "DELETE FROM Source WHERE ROWID=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, source->ID());
	
	sqlite3_step(statement);
	
	sqlite3_finalize(statement);
}

#pragma mark - Donnee



#pragma mark - Static functions


#pragma mark - Fully specialized template functions

