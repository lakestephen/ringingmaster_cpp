#pragma once

class Method;
class ProofInput;
class MethodAnalysis;
class Row;


class ProofAnalysisEngine
{
public:
	ProofAnalysisEngine(const ProofInput& proofInput, const Method& method, MethodAnalysis& methodAnalysis);
	~ProofAnalysisEngine(void);

	void doCalculate();
	bool hasActuallyTerminated();
	void requestEarlyTerminate();



protected:
	void calculate();
	Row** getSortedRows(int& count);
	static int compareRows( const void * arg1, const void * arg2 );
	void checkFalsenessSingleBlock(Row** sortedRows, int size);
	void checkFalsenessMultiBlock(Row** sortedRows, int size);
	void setupMusic();
	void checkMusic(Row **rows, int size);
	void checkRowForMusic(Row* row);
	void setupDisplay();

	//inputs
	const ProofInput& _ip;
	const Method& _met;
	MethodAnalysis& _anal;

	//internal data
	bool _hasActuallyTerminated;
	bool _hasRequestedTerminate;


};
