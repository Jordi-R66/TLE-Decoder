#pragma once
#include "Common.h"

typedef char* cstring;

struct record {
	int32_t time;
	double* values;
	size_t n_values;
};

typedef struct record record_t;

struct file {
	cstring filename;
	record_t* records;

	size_t n_records;
	size_t nextIndex;

	cstring* columnNames;
	size_t n_columns;
};

typedef struct file file_t;

file_t createFileObject(cstring filename, size_t n_records);
record_t createRecord(int32_t time, double* values, size_t n_values);

void addRecord(file_t* file, record_t record);
void freeFile(file_t* file);
void freeRecord(record_t record);
void writeFile(file_t* file);
