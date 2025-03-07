#include "logger.h"

record_t createRecord(int32_t time, double* values, size_t n_values) {
	record_t record;
	record.time = time;
	record.values = values;

	return record;
}

file_t createFileObject(string* filename, size_t n_records) {
	file_t file;

	file.filename = filename;
	file.records = (record_t*)calloc(n_records, sizeof(record_t));

	if (file.records == NULL) {
		fprintf(stderr, "Error while allocating memory for records\n");
		exit(EXIT_FAILURE);
	}

	file.n_records = n_records;
	file.nextIndex = 0;

	FILE* fp = fopen(filename, "w");

	if (fp == NULL) {
		free(file.records);
		fprintf(stderr, "Error while creating the file\n");
		exit(EXIT_FAILURE);
	}

	return file;
}

void addRecord(file_t* file, record_t record) {
	if (file->nextIndex >= file->n_records) {
		fprintf(stderr, "Error: file is full\n");
		return;
	}

	file->records[file->nextIndex] = record;
	file->nextIndex++;
}

void freeFile(file_t* file) {
	free(file->records);
	free(file->filename);
}

void freeRecord(record_t record) {
	free(record.values);
}

void writeFile(file_t* file) {
	FILE* fp = fopen(file->filename, "w");

	if (fp == NULL) {
		fprintf(stderr, "Error while opening the file\n");
		return;
	}

	for (size_t i = 0; i < file->nextIndex; i++) {
		record_t record = file->records[i];
		fprintf(fp, "%d ", record.time);

		for (size_t j = 0; j < record.n_values; j++) {
			fprintf(fp, "%f ", record.values[j]);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
}