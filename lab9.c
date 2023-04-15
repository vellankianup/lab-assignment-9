#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType record;
    int isOccupied;

};

// Compute the hash function
int hash(int x)
{
	return x % 100;

}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i, j;
    struct RecordType record;

    for (i = 0; i < 11; ++i)
    {
        if (pHashArray[i].isOccupied)
        {
            printf("Index %d ->", i);

            record = pHashArray[i].record;
            printf(" %d, %c, %d", record.id, record.name, record.order);

            j = i + 1;
            while (j < hashSz && pHashArray[j].isOccupied && hash(pHashArray[j].record.id) == i)
            {
                record = pHashArray[j].record;
                printf(" -> %d, %c, %d", record.id, record.name, record.order);
                j++;
            }
            printf(" -> NULL\n");
        }
        else
        {
            printf("Index %d -> NULL\n", i);
        }
    }
}

int main(void)

{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash array
    int hashSz = recordSz;
    struct HashType *pHashArray = (struct HashType*) malloc(sizeof(struct HashType) * 11);
    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory for hash array\n");
        exit(-1);
    }
     memset(pHashArray, 0, sizeof(struct HashType) * hashSz);

    // Insert records into the hash array
    int i;
    for (i = 0; i < recordSz; i++)
    {
        int idx = hash(pRecords[i].id) % hashSz;
        struct HashType *pHash = pHashArray + idx;
        if (pHash->isOccupied == 0)
        {
            
            pHash->record.id = pRecords[i].id;
            pHash->record.name = pRecords[i].name;
            pHash->record.order = pRecords[i].order;
            pHash->isOccupied = 1;
        }
        else
        {
            
            while (pHash->isOccupied != 0)
            {
                ++idx;
                if (idx == hashSz)
                {
                    idx = 0;
                }
                pHash = pHashArray + idx;
            }
            pHash->record.id = pRecords[i].id;
            pHash->record.name = pRecords[i].name;
            pHash->record.order = pRecords[i].order;
            pHash->isOccupied = 1;
        }
    }

    // Display records in the hash array
    displayRecordsInHash(pHashArray, hashSz);

    free(pHashArray);
    free(pRecords);

    return 0;
}
