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

#include "Utilities.h"

#include <iostream>
#include <assert.h>
#include <boost/lexical_cast.hpp>

#include "CommunicationProtocol.h"

template<typename T>
class Source;
template<typename T>
class Donnee;

class Record;

class GenericSource;

class VitesseRotation;

#include "DatabaseData.h"

class Database
{
public:
	
#pragma mark - Fonctions utiles
	
	static std::vector<uint8_t> toVector(const DatabaseData& value);
	static DatabaseData* fromVector(std::vector<uint8_t>& value,DataType type);
	
#pragma mark - Configuration Field
	
	template<typename T>
	static void setConfigurationField(std::string field, T value);
	static DatabaseData* getConfigurationField(std::string field);
	static bool hasConfigurationField(std::string field);
	
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
	static Record* recordWithId(sqlite_int64 ID);
	
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
	
	template<typename T>
	static Donnee<T>* donneeForSourceAtIndex(const Source<T>* source, std::size_t index);
	
#pragma mark - Motor Command
	
	static CommandeVitesse commandForRequestedMotorSpeed(const VitesseRotation& vitesseRotation);
	static void saveCommandForMotorSpeed(const VitesseRotation& vitesseRotation, const CommandeVitesse commande);
	
private:
	Database();
	~Database();
	
	sqlite3* database;
	
	static Database& shared();
	
	template<typename T>
	void setConfigurationField_(std::string& field, T data);
	
	DatabaseData* getConfigurationField_(std::string& field);
	std::vector< std::pair<std::string,DataType> > getAllConfigurationFields_();
	void removeConfigurationField_(std::string field);
	int indexOfField_(std::string field);
	bool hasConfigurationField_(std::string field);
	
	template<typename T>
	bool findData_(Donnee<T>& donnee);
	template<typename T,typename S>
	void saveDataWithValue_(T& value, Donnee<S>& donnee);
	template<typename T>
	void saveDataWithValue_(std::vector<uint8_t> value, Donnee<T>& donnee);
	
	template<typename T>
	Donnee<T>* donneeForSourceAtIndex_(const Source<T>* source, std::size_t index);
	
	template<typename T>
	void saveSource_(Source<T>& source);
	std::vector<GenericSource*> getAllSourcesForRecord_(Record* record);
	void removeSource_(GenericSource* source);
	
	std::vector<Record*> getAllRecords_();
	void saveRecord_(Record* record);
	void removeRecord_(Record* record);
	Record* recordWithId_(sqlite_int64 ID);
	
	std::string changeConfigurationFieldName_(std::string oldFieldName, std::string newFieldName);
	
	CommandeVitesse commandForRequestedMotorSpeed_(const VitesseRotation& vitesseRotation);
	void saveCommandForMotorSpeed_(const VitesseRotation& vitesseRotation, const CommandeVitesse commande);

};

#pragma mark - Implementation

#include "DatabaseData.h"

#include <boost/date_time.hpp>

#include "Donnee.h"
#include "Source.h"

#include "NumericValue.h"

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
		cmd = "UPDATE Donnee SET value=?001 WHERE ROWID=?002";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_blob(statement, 1, value.data(), value.size(), SQLITE_STATIC);
		sqlite3_bind_int64(statement, 2, donnee.ID());
		
		sqlite3_step(statement);
	}
	else
	{
		cmd = "INSERT INTO Donnee(date,sourceID,value) VALUES (?001,?002,?003)";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		ptime ref(date(2001,Jan,1));
		
		boost::int64_t offset = (donnee.date()-ref).ticks();
		
		sqlite3_bind_int64(statement, 1, offset);
		sqlite3_bind_int64(statement, 2, donnee.source()->ID());
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
	sqlite3_bind_int64(statement, 2, donnee.source()->ID());
	
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

template<typename T>
Donnee<T>* Database::donneeForSourceAtIndex(const Source<T>* source, std::size_t index)
{
	return Database::shared().donneeForSourceAtIndex_(source,index);
}

template<typename T>
Donnee<T>* Database::donneeForSourceAtIndex_(const Source<T>* source, std::size_t index)
{
	std::string cmd;
	
	cmd = "SELECT date,value FROM Donnee WHERE sourceID=?001 LIMIT 1 OFFSET ?002";
	
	sqlite3_stmt* statement;
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, source->ID());
	sqlite3_bind_int64(statement, 2, index);
	
	if(sqlite3_step(statement) == SQLITE_ROW)
	{
		boost::int64_t offset = sqlite3_column_int64(statement,0);
		
		time_duration offsetTime = microsecondsTimeDuration(offset);
		
		ptime newDate(date(2001,Jan,1));
		newDate+=offsetTime;
		
		uint8_t* buffer = (uint8_t*)sqlite3_column_blob(statement, 1);
		std::size_t length = sqlite3_column_bytes(statement, 1);
		
		std::vector<uint8_t> res(buffer,buffer+length);
		
		sqlite3_finalize(statement);
		
		return new Donnee<T>(source,newDate,DatabaseData::dataWithType(res, source->type()));
	}
	else
	{
		sqlite3_finalize(statement);
		return NULL;
	}
}

template<typename T>
void Database::setConfigurationField(std::string field, T value)
{
	return Database::shared().setConfigurationField_(field, value);
}

template<typename T>
void Database::setConfigurationField_(std::string& field, T data)
{
	std::string cmd;
	
	cmd = "SELECT ROWID FROM Configuration WHERE field=?001";
	
	sqlite3_stmt* statement;
	
	std::vector<uint8_t> value(data->toBinary());
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_text(statement, 1, field.c_str(), field.length(), SQLITE_STATIC);
	
	if(sqlite3_step(statement) == SQLITE_ROW)
	{
		sqlite_int64 ROWID = sqlite3_column_int64(statement, 0);
		
		sqlite3_finalize(statement);
		
		cmd = "UPDATE Configuration SET value=?001,dataType=?002 WHERE ROWID=?003";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_blob(statement, 1, value.data(), value.size(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 2, typeOfTemplate(data));
		sqlite3_bind_int64(statement, 3, ROWID);
		
		sqlite3_step(statement);
	}
	else
	{
		sqlite3_finalize(statement);
		
		cmd = "INSERT INTO Configuration(field,value,dataType) VALUES (?001,?002,?003)";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_text(statement, 1, field.c_str(), field.length(), SQLITE_STATIC);
		sqlite3_bind_blob(statement, 2, value.data(), value.size(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 3, typeOfTemplate(data));
		
		sqlite3_step(statement);
	}
	
	sqlite3_finalize(statement);
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
		cmd = "UPDATE Source SET name=?001,dataType=?002,size=?004,recordId=?005 WHERE ROWID=?003";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_text(statement, 1, source.name().c_str(), source.name().length(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 2, source.type());
		sqlite3_bind_int64(statement, 3, sourceId);
		sqlite3_bind_int64(statement, 4, source.size());
		sqlite3_bind_int64(statement, 5, source.recordID());
		
		sqlite3_step(statement);
	}
	else
	{
		cmd = "INSERT INTO Source(name,dataType,size,recordId) VALUES (?001,?002,?003,?004)";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
		
		sqlite3_bind_text(statement, 1, source.name().c_str(), source.name().length(), SQLITE_STATIC);
		sqlite3_bind_int(statement, 2, source.type());
		sqlite3_bind_int64(statement, 3, source.size());
		sqlite3_bind_int64(statement, 4, source.recordID());
		
		sqlite3_step(statement);
		
		source.setID(sqlite3_last_insert_rowid(database));
	}
	
	sqlite3_finalize(statement);
}

template<typename T>
T Database::getConfigurationFieldValue(std::string field)
{
	// We assume T is a numeric type here
	
	DatabaseData* data = getConfigurationField(field);
	
	T res;
	
	switch(data->type()) 
	{
		case TYPE_DOUBLE:
		{
			NumericValue<double>* value = (NumericValue<double>*)data;
			res = value->value();
			break;
		}
			
		case TYPE_FLOAT:
		{
			NumericValue<float>* value = (NumericValue<float>*)data;
			res = value->value();
			break;
		}
			
		case TYPE_INT:
		{
			NumericValue<int>* value = (NumericValue<int>*)data;
			res = value->value();
			break;
		}
			
		case TYPE_INT64:
		{
			NumericValue<int64_t>* value = (NumericValue<int64_t>*)data;
			res = value->value();
			break;
		}
			
		default:
		{
			std::cout << "Type non supporté" << std::endl;
			assert(0);
			return T();
			break;
		}
	}
	
	delete data;
	
	return res;
}

#endif
