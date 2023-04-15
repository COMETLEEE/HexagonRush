#include "pch.h"
#include "ASEReader.h"

#include "ASEToken.h"
#include "StringHelper.h"

namespace DataLoader
{
	ASEReader::ASEReader() :
		_file(nullptr), _bufferIndex(0), _tokenIndex(0)
	{
		
	}

	ASEReader::~ASEReader()
	{
		if (_file != nullptr)
			Close();
	}

	void ASEReader::Release()
	{
		Close();
	}

	bool ASEReader::Open(const tstring& path)
	{
		fopen_s(&_file, StringHelper::ToString(path).c_str(), "r");

		_bufferIndex = 0;

		_tokenIndex = 0;

		return _file != nullptr ? true : false;
	}

	void ASEReader::ReadNextLine()
	{
		fgets(_lineBuffer, BUFFER_SIZE, _file);
	}

	bool ASEReader::IsEOF()
	{
		return feof(_file);
	}

	void ASEReader::Close()
	{
		if (_file != nullptr)
			fclose(_file);

		_file = nullptr;
	}

	uint32 ASEReader::ParsingInt()
	{
		char character = _lineBuffer[_bufferIndex++];

		// 화이트 스페이스를 띄워주자
		while ((character == ' ') || (character == '\t') || (character == '\n') || (character == '\r'))
		{
			character = _lineBuffer[_bufferIndex++];
		}

		_token[0] = character;

		_tokenIndex = 1;

		while ((character >= '0') && (character <= '9') || (character == '-'))
		{
			character = _lineBuffer[_bufferIndex++];

			_token[_tokenIndex++] = character;
		}

		// 항상 말하지만 문자열의 끝은 널 문자
		_token[_tokenIndex] = NULL;

		return atoi(_token);
	}

	float ASEReader::ParsingFloat()
	{
		char character = _lineBuffer[_bufferIndex++];

		// 화이트 스페이스를 띄워주자
		while ((character == ' ') || (character == '\t') || (character == '\n') || (character == '\r'))
		{
			character = _lineBuffer[_bufferIndex++];
		}

		_token[0] = character;

		_tokenIndex = 1;

		while ((character >= '0') && (character <= '9') || (character == '-') || (character == '.'))
		{
			character = _lineBuffer[_bufferIndex++];
			_token[_tokenIndex++] = character;
		}

		// 항상 말하지만 문자열의 끝은 널 문자
		_token[_tokenIndex] = NULL;

		return static_cast<float>(atof(_token));
	}

	std::string ASEReader::ParsingString()
	{
		char character = _lineBuffer[_bufferIndex++];

		// 화이트 스페이스를 띄워주자
		// 빈칸은 넣어주고 싶은데 ..
		while ((character == ' ') || (character == '\t') || (character == '\n') || (character == '\r'))
		{
			character = _lineBuffer[_bufferIndex++];
		}

		// "는 빼고 기록하자.
		_token[0] = _lineBuffer[_bufferIndex++];

		_tokenIndex = 1;

		character = _lineBuffer[_bufferIndex++];

		// 문장 끝까지 기록하자. "는 빼고.
		while ((character != '"'))
		{
			_token[_tokenIndex++] = character;

			character = _lineBuffer[_bufferIndex++];
		}

		// 문자열의 마지막은 NULL문자.
		_token[_tokenIndex] = NULL;

		return std::string(_token);
	}

	uint32 ASEReader::ParsingReserveWord()
	{
		// 문자열 읽기 시작 작업
		_tokenIndex = 0;

		// 문자열 읽기 시작 작업
		_bufferIndex = 0;

		char character = '1';

		// 1. 줄을 읽으면서 *이 나오면 *다음부터 나오는 문자를 포함해 전부 기록한다.
		while (character != '\0')
		{
			character = _lineBuffer[_bufferIndex++];

			// 화이트 스페이스는 무시한다.
			if ((character == ' ') || (character == '\t') || (character == '\r') || (character == '\n'))
				continue;
			// '*' 찾았다 ..!
			else if ((character == '}') || (character == '\0'))
			{
				return RESERVEDWORD_END;
			}
			else
			{
				_token[_tokenIndex++] = '*';
				break;
			}
		}

		//// 2. 이제 저 위의 문장을 뚫고 오면 공백이 아닌 것이 시작되었다는 뜻. 즉, 토큰을 검색하면 된다 ..!
		//// 예외 : 그러나, '}' 이라면 그냥 해당 예약어에 대한 블록이 끝났따는 것이므로 끝내야한다.
		//if ((_lineBuffer[_bufferIndex] == '}') || (character == '\0'))
		//	return RESERVEDWORD_END;

		while ((_lineBuffer[_bufferIndex] != ' ') && (_lineBuffer[_bufferIndex] != '\t'))
		{
			_token[_tokenIndex++] = _lineBuffer[_bufferIndex++];
		}

		// 문자열의 맨 마지막은 널 문자 ..!
		_token[_tokenIndex] = NULL;

		unsigned int reservedIndex = 0;

		// 3. Reserved Word 검색
		for (reservedIndex = 0; reservedIndex < RESERVEDWORD_COUNT; reservedIndex++)
		{
			if (!strcmp(_token, ReservedTokens[reservedIndex]))
				break;
		}

		return reservedIndex;
	}

	DirectX::SimpleMath::Vector3 ASEReader::ParsingVector3()
	{
		Vector3 ret;

		// y, z를 바꿔서 파싱한다. (좌표계가 다르기 때문이다.)
		ret.x = ParsingFloat();
		ret.z = ParsingFloat();
		ret.y = ParsingFloat();

		return ret;
	}

	void ASEReader::TapJump()
	{
		char character = _lineBuffer[_bufferIndex++];

		// 화이트 스페이스를 띄워주자
		while ((character == ' ') || (character == '\t') || (character == '\n') || (character == '\r'))
			character = _lineBuffer[_bufferIndex++];

		_bufferIndex++;
	
		character = _lineBuffer[_bufferIndex++];

		// 문장 끝까지 기록하자. "는 빼고.
		while ((character != '"') && (character != ' '))
			character = _lineBuffer[_bufferIndex++];
	}
}