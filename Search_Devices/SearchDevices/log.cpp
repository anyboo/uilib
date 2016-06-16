#include "log.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/Path.h"

using Poco::Logger;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::Path;


namespace Loggering
{

	void Logger_initiation()
	{
		AutoPtr<FileChannel> file_channel(new FileChannel());
		file_channel->setProperty("rotation", "10M");
		file_channel->setProperty("archive", "timestamp");
		std::string strpath = Path::current();			
		strpath.append("log\\searchdevice.log");
		file_channel->setProperty("path", strpath);
		//AutoPtr<PatternFormatter> pattern_formatter(new PatternFormatter("%L%H:%M:%S-code line :%u-%U : %t"));
		AutoPtr<PatternFormatter> pattern_formatter(new PatternFormatter("%Y-%m-%d %H:%M:%S %s [%p] %U(%u): %t"));
		AutoPtr<FormattingChannel> formatter_channle(new FormattingChannel(pattern_formatter, file_channel));
		Logger::root().setChannel(formatter_channle);
		///- finish logger initiation
	}

	void Setup_logger()
	{
		static bool b_setup = false;     // only allow run once time 
		if (!b_setup)
		{
			b_setup = true;
			Logger_initiation();
		}
	}
}