#include <lutil/util/StrUtil.hxx>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/functional/hash.hpp>

#include <sstream>

using namespace LUtil;
//----------------------------------------------------------------------------
const std::string LUtil::StrUtil::sEmpty = "";
#if _WIN32
const std::wstring LUtil::StrUtil::sWEmpty = L"";
#endif
//----------------------------------------------------------------------------
std::string StrUtil::generateUUIDStr(bool useHash)
{
	std::string value(LUtil::StrUtil::sEmpty);

	boost::uuids::uuid uuid = boost::uuids::random_generator()();

	if (useHash)
	{
		std::size_t hashValue = boost::uuids::hash_value(uuid);
		std::stringstream ss;
		ss << hashValue;
		value = ss.str();
		ss.str("");
	}
	else
	{
		value = boost::uuids::to_string(uuid);
	}

	return value;
}
//----------------------------------------------------------------------------