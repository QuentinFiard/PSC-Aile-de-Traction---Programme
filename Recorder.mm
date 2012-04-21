//
//  Recorder.cpp
//  PSC
//
//  Created by Quentin Fiard on 09/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Recorder.h"

#include "Camera.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

static Recorder* shared_ = NULL;

Recorder::Recorder()
{
	currentRecord = NULL;
}

Recorder* Recorder::shared()
{
	if(shared_==NULL)
	{
		shared_ = new Recorder();
	}
	return shared_;
}

bool Recorder::isRecording()
{
	return shared()->isRecording_();
}

bool Recorder::isRecording_()
{
	return (currentRecord!=NULL);
}

void Recorder::startRecording()
{
	shared()->startRecording_();
}

void Recorder::startRecording_()
{
	currentRecord = new Record(microsec_clock::local_time());
	currentRecord->save();
	Camera::startTracking();
}

void Recorder::stopRecording()
{
	shared()->stopRecording_();
}

void Recorder::stopRecording_()
{
	delete currentRecord;
	currentRecord = NULL;
	Camera::stopTracking();
}

Record* Recorder::record()
{
	return shared()->currentRecord;
}