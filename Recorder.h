//
//  Recorder.h
//  PSC
//
//  Created by Quentin Fiard on 09/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Recorder_h
#define PSC_Recorder_h

#include "Record.h"

class Recorder
{
public:
	
	static bool isRecording();
	static void startRecording();
	static void stopRecording();
	
	static Record* record();
	
protected:
	
	Recorder();
	
	static Recorder* shared();
	
	bool isRecording_();
	void startRecording_();
	void stopRecording_();
	
private:
	
	Record* currentRecord;
	
};

#endif
