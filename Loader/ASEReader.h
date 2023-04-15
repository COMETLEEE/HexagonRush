#pragma once

namespace DataLoader
{
	constexpr uint32 BUFFER_SIZE = 512;

	constexpr uint32 RESERVEDWORD_END = 65536;

	constexpr uint32 RESERVEDWORD_COUNT = 233;

	/// <summary>
	/// ASE Data Parsing 중 파일을 순서대로 읽으면서구문을 읽어 들이는 역할을 한다.
	/// </summary>
	class ASEReader
	{
	public:
		ASEReader();

		~ASEReader();

	private:
		FILE* _file;

		char _lineBuffer[BUFFER_SIZE] = { 0 };

		uint32 _bufferIndex;

		char _token[BUFFER_SIZE] = { 0 };

		uint32 _tokenIndex;

	public:
		void Release();

		// 파일을 오픈한다.
		bool Open(const tstring& path);

		// 다음 줄로 넘어간다.
		void ReadNextLine();

		bool IsEOF();

		void Close();

		// 정수 (양수 또는 음수) 형태의 단어를 읽는다.
		uint32 ParsingInt();

		// 소수 (양수 또는 음수) 형태의 단어를 읽는다.
		float ParsingFloat();

		// 문자열을 파싱 후 "std::string" 형태로 반환한다.
		std::string ParsingString();

		// Float Vector를 파싱함. (y, z 바뀌어 있지 않아욧)
		Vector3 ParsingVector3();

		// 예약어 토큰을 파싱하고 배열에 미리 리스트해놓은 것 중에서 어떤 명령어인지 반환한다.
		uint32 ParsingReserveWord();

		// ASE 파일 내에서 필요없는 문자열을 대상으로 스킵한다.
		void TapJump();
	};
}