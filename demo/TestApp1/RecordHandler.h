#pragma once

enum ENCODE{ FLV, MP4, };

class CRecordHandler
{
public:

	CRecordHandler();
	~CRecordHandler();

	static CRecordHandler& Inst();

	void start();
	void stop();
	void pause();
	void resume();
	void SetVolume(bool mute = false);
	void SetMicro(bool mute = false);
	void SetEncode(const ENCODE& type);
	void SetArea(const POINT& p, const SIZE& s);

protected:

	void init();
	void uninit();

private:
	CRecordHandler(CRecordHandler&);
	CRecordHandler operator= (const CRecordHandler&);

	bool MuteVolume;
	bool MuteMicroPhone;
	bool Recording;
	std::string format;
	std::string file;
	POINT _p;
	SIZE _s;

	size_t count;
};

