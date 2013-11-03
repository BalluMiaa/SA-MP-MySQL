#pragma once
#ifndef INC_CCALLBACK_H
#define INC_CCALLBACK_H


#include <list>
#include <stack>
#include <string>
#include <boost/lockfree/queue.hpp>
#include <boost/variant.hpp>

using std::list;
using std::stack;
using std::string;

#include "main.h"


class CMySQLQuery;


class CCallback 
{
public:

	void FillCallbackParams(AMX* amx, cell* params, const char *param_format, const int ConstParamCount);

	CCallback() :
		IsInline(false)
	{}
	~CCallback() {}


	stack<boost::variant<cell, string> > Parameters;
	string Name;
	string ParamFormat;
	bool IsInline;

	
	static void ProcessCallbacks();
	
	static inline void AddQueryToQueue(CMySQLQuery *cb) 
	{
		m_CallbackQueue.push(cb);
	}
	static inline CMySQLQuery *GetNextQuery() 
	{
		CMySQLQuery *NextQuery = NULL;
		m_CallbackQueue.pop(NextQuery);
		return NextQuery;
	}

	static void AddAmx(AMX *amx);
	static void EraseAmx(AMX *amx);

	static void ClearAll();

private:
	static boost::lockfree::queue<
			CMySQLQuery*, 
			boost::lockfree::fixed_sized<true>,
			boost::lockfree::capacity<8192>
		> m_CallbackQueue;

	static list<AMX *> m_AmxList;
};


#endif // INC_CCALLBACK_H
