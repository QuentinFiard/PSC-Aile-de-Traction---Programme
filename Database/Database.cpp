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

#include "StringValue.h"

#include "VitesseRotation.h"

using namespace boost::posix_time;
using namespace boost::gregorian;


#pragma mark - Shared Database Object

static Database* shared_ = NULL;

#pragma mark - Fonctions utiles

std::vector<uint8_t> Database::toVector(const DatabaseData& value)
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
		
		time_duration offsetTime = microsecondsTimeDuration(offset);
		
		ptime date(ref);
		date+=offsetTime;
		
		Record* tmp = new Record(ID,date,tag);
		
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
	sqlite3_stmt* statement, *statement2;
	
	std::string cmd;
	
	cmd = "DELETE FROM Record WHERE ROWID=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, record->ID());
	
	sqlite3_step(statement);
	
	sqlite3_finalize(statement);
	
	cmd = "SELECT ROWID FROM Source WHERE recordId=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, record->ID());
	
	while(sqlite3_step(statement) == SQLITE_ROW)
	{
		sqlite_int64 sourceID = sqlite3_column_int64(statement, 0);
		
		cmd = "DELETE FROM Source WHERE ROWID=?001";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement2, NULL);
		
		sqlite3_bind_int64(statement2, 1, sourceID);
		
		sqlite3_step(statement2);
		
		sqlite3_finalize(statement2);
		
		cmd = "DELETE FROM Donnee WHERE sourceID=?001";
		
		sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement2, NULL);
		
		sqlite3_bind_int64(statement2, 1, sourceID);
		
		sqlite3_step(statement2);
		
		sqlite3_finalize(statement2);
	}
	
	sqlite3_finalize(statement);
}

Record* Database::recordWithId(sqlite_int64 ID)
{
	Database::shared().recordWithId_(ID);
}

Record* Database::recordWithId_(sqlite_int64 ID)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT startDate,tag FROM Record WHERE ROWID=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	ptime ref(date(2001,Jan,1));
	
	if(sqlite3_step(statement) == SQLITE_ROW)
	{
		boost::int64_t offset = sqlite3_column_int64(statement,0);
		std::string tag((const char*)sqlite3_column_text(statement,1));
		
		time_duration offsetTime = microsecondsTimeDuration(offset);
		
		ptime date(ref);
		date+=offsetTime;
		
		Record* tmp = new Record(ID,date,tag);
		
		return tmp;
	}
	else
	{
		sqlite3_finalize(statement);
		return NULL;
	}
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
	
	cmd = "SELECT ROWID,name,dataType,size FROM Source WHERE recordId=?001 ORDER BY name ASC";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite_int64 recordId = record->ID();
	
	sqlite3_bind_int64(statement, 1, recordId);
	
	while(sqlite3_step(statement) == SQLITE_ROW)
	{
		sqlite_int64 ID = sqlite3_column_int64(statement, 0);
		std::string name((const char*)sqlite3_column_text(statement, 1),sqlite3_column_bytes(statement, 1));
		DataType type = sqlite3_column_int(statement, 2);
		std::size_t size = sqlite3_column_int64(statement, 3);
		
		res.push_back(GenericSource::sourceWithIDNameTypeRecordIDAndSize(ID, name, type, record,size));
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
	
	cmd = "DELETE FROM Donnee WHERE sourceID=?001";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_int64(statement, 1, source->ID());
	
	sqlite3_step(statement);
	
	sqlite3_finalize(statement);
}

#pragma mark - Donnee

#pragma mark - Motor command

CommandeVitesse Database::commandForRequestedMotorSpeed(const VitesseRotation& vitesseRotation)
{
	return Database::shared().commandForRequestedMotorSpeed_(vitesseRotation);
}

CommandeVitesse Database::commandForRequestedMotorSpeed_(const VitesseRotation& vitesseRotation)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "SELECT signal,speed FROM MotorSpeed ORDER BY ABS(speed-?001) ASC LIMIT 2";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_double(statement, 1, vitesseRotation.speed());
	
	if(!sqlite3_step(statement))
	{
		std::cout << "Il est nécessaire de remplir le tableau Vitesse de Rotation / Signal pour utiliser le moteur\n";
		assert(0);
	}
	
	double signal1 = sqlite3_column_double(statement, 0);
	double speed1 = sqlite3_column_double(statement, 1);
	
	if(!sqlite3_step(statement))
	{
		std::cout << "Le tableau Vitesse de Rotation / Signal ne contient qu'une seule valeur\n";
		assert(0);
	}
	
	double signal2 = sqlite3_column_double(statement, 0);
	double speed2 = sqlite3_column_double(statement, 1);
	
	sqlite3_finalize(statement);
	
	if(speed1==speed2)
	{
		std::cout << "Deux vitesses identiques dans le tableau -> problème pour l'interpolation\n";
		assert(0);
	}
	
	CommandeVitesse res;
	
	res.nonAsservie.signal = ((signal2-signal1)/(speed2-speed1))*(vitesseRotation.speed() - speed1) + signal1;
	
	if(res.nonAsservie.signal > 1)
	{
		res.nonAsservie.signal = 1;
	}
	
	if(res.nonAsservie.signal < -1)
	{
		res.nonAsservie.signal = -1;
	}
	
	return res;
}

void Database::saveCommandForMotorSpeed(const VitesseRotation& vitesseRotation, const CommandeVitesse commande)
{
	Database::shared().saveCommandForMotorSpeed_(vitesseRotation, commande);
}

void Database::saveCommandForMotorSpeed_(const VitesseRotation& vitesseRotation, const CommandeVitesse commande)
{
	sqlite3_stmt* statement;
	
	std::string cmd;
	
	cmd = "INSERT INTO MotorSpeed(speed,signal) VALUES (?001,?002)";
	
	sqlite3_prepare_v2(database, cmd.c_str(), cmd.length(), &statement, NULL);
	
	sqlite3_bind_double(statement,1,vitesseRotation.speed());
	sqlite3_bind_double(statement,2,commande.nonAsservie.signal);
	
	sqlite3_step(statement);
	
	sqlite3_finalize(statement);
}

#pragma mark - Static functions


#pragma mark - Fully specialized template functions

template<>
std::string Database::getConfigurationFieldValue<std::string>(std::string field)
{
	DatabaseData* res = Database::getConfigurationField(field);
	
	if(res->type() != TYPE_STRING)
	{
		std::cout << "Type incompatible";
		assert(0);
	}
	
	StringValue* stringValue = (StringValue*)res;
	
	std::string resString = stringValue->valueToString();
	
	delete res;
	
	return resString;
}
