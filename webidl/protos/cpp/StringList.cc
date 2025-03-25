#include "StringList.h"
#include <cstring>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

//StringList::StringList(std::vector<std::string> v){}

int StringList::add(char **a) {
	int i = 0;
	while (a && a[i] != NULL) {
		sv.push_back(a[i]);	
		i++;
	}
	return i;
}

int StringList::add_json(const char *j) {
	if (!j) return 0;
	int k = 0;
	rapidjson::Document d;
	d.Parse(j);
	if (d.IsArray()) {
		for (rapidjson::SizeType i = 0; i < d.Size(); i++, k++) {
			if (d[i].IsString())
				sv.push_back(d[i].GetString());	
		}
		return k;
	} else {
		return 0;
	}
}

char * StringList::json()
{
	rapidjson::Document d;
	d.SetArray();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	for (auto& i : sv) {
		printf("jadd %s\n", i.c_str());
		d.PushBack(rapidjson::StringRef(i.c_str(), i.size()), allocator);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	const char* output = buffer.GetString();
	return strdup(output);

}

char* StringList::get(size_t i)
{
	static std::string nulls("");
	if (i < sv.size() && i >= 0)
	       return strdup(sv[i].c_str());
       	else
	       return NULL;
}

size_t StringList::size() { return sv.size(); }

