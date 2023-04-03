#pragma once
#ifndef _SERIALIZATION_BINARY_H_
#define _SERIALIZATION_BINARY_H_

#include<string>
#include<string_view>
#include<fstream>
#include<memory>

namespace szon
{
	class SerializationBinary
	{
	public:
		SerializationBinary();

		virtual ~SerializationBinary() = default;

		SerializationBinary(const SerializationBinary& other) = delete;

#pragma region part with arrays

		template<typename T>
		void SerializeArrayOfData(T* data, const int size, std::string_view path);

		template<typename T>
		void DeserializeArrayOfData(T* data, const int size, std::string_view path);

		template<>
		void SerializeArrayOfData<std::string>(std::string* data, const int size, std::string_view path);

		template<>
		void DeserializeArrayOfData<std::string>(std::string* data, const int size, std::string_view path);

		template<>
		void SerializeArrayOfData<std::wstring>(std::wstring* data, const int size, std::string_view path);

		template<>
		void DeserializeArrayOfData<std::wstring>(std::wstring* data, const int size, std::string_view path);

		template<>
		void SerializeArrayOfData<wchar_t>(wchar_t* data, const int size, std::string_view path);

		template<>
		void DeserializeArrayOfData<wchar_t>(wchar_t* data, const int size, std::string_view path);

#pragma endregion

#pragma region part with one data type

		template<typename T>
		void SerializeData(const T& data, std::string_view path);

		template<typename T>
		void DeserializeData(T& data, std::string_view path);

		template<>
		void SerializeData<std::string>(const std::string& data, std::string_view path);

		template<>
		void DeserializeData<std::string>(std::string& data, std::string_view path);

		template<>
		void SerializeData<std::wstring>(const std::wstring& data, std::string_view path);

		template<>
		void DeserializeData<std::wstring>(std::wstring& data, std::string_view path);

		template<>
		void SerializeData<wchar_t>(const wchar_t& data, std::string_view path);

		template<>
		void DeserializeData<wchar_t>(wchar_t& data, std::string_view path);

#pragma endregion

#pragma region other

		bool IsFileEmpty(std::string_view path);

		void ResetFileReadPosition();

		void ClearFile(std::string_view path);

#pragma endregion

	private:
		std::uint64_t m_current_position;

		std::ofstream m_fout;
		std::ifstream m_fin;

		std::wofstream m_wfout;
		std::wifstream m_wfin;
	};

#pragma region Implementation

	SerializationBinary::SerializationBinary()
	{
		m_current_position = NULL;
		m_fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		m_fout.exceptions(std::ofstream::badbit | std::ofstream::failbit);
	}

#pragma region part with arrays

	template<typename T>
	void SerializationBinary::SerializeArrayOfData(T* data, const int size, std::string_view path)
	{
		m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (T* pArr = data; pArr < data + size; ++pArr)
		{
			m_fout.write(reinterpret_cast<const char*>(pArr), sizeof(T));
		}

		m_fout.close();
	}

	template<typename T>
	void SerializationBinary::DeserializeArrayOfData(T* data, const int size, std::string_view path)
	{
		m_fin.open(path.data(), std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (T* pArr = data; pArr < data + size; ++pArr)
		{
			m_fin.read(reinterpret_cast<char*>(pArr), sizeof(T));
		}

		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	template<>
	void SerializationBinary::SerializeArrayOfData<std::string>(std::string* data, const int size, std::string_view path)
	{
		m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (std::string* pArr = data; pArr < data + size; ++pArr)
		{
			std::size_t data_lenght = (*pArr).length() + 1;

			m_fout.write(reinterpret_cast<const char*>(&data_lenght), sizeof(data_lenght));
			m_fout.write(reinterpret_cast<const char*>((*pArr).c_str()), data_lenght);
		}

		m_fout.close();
	}

	template<>
	void SerializationBinary::DeserializeArrayOfData<std::string>(std::string* data, const int size, std::string_view path)
	{
		m_fin.open(path.data(), std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (std::string* pArr = data; pArr < data + size; ++pArr)
		{
			std::size_t data_lenght;

			m_fin.read(reinterpret_cast<char*>(&data_lenght), sizeof(data_lenght));
			auto buffer = std::make_unique<char[]>(data_lenght);

			m_fin.read(buffer.get(), data_lenght);
			*pArr = buffer.get();
		}

		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	template<>
	void SerializationBinary::SerializeArrayOfData<std::wstring>(std::wstring* data, const int size, std::string_view path)
	{
		m_wfout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (!m_wfout.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (std::wstring* pArr = data; pArr < data + size; ++pArr)
		{
			std::size_t data_lenght = (*pArr).length() + 1;

			m_wfout.write(reinterpret_cast<const wchar_t*>(&data_lenght), sizeof(data_lenght) / sizeof(wchar_t));
			m_wfout.write(reinterpret_cast<const wchar_t*>((*pArr).c_str()), data_lenght);
		}

		m_wfout.close();
	}

	template<>
	void SerializationBinary::DeserializeArrayOfData<std::wstring>(std::wstring* data, const int size, std::string_view path)
	{
		m_wfin.open(path.data(), std::ios::binary | std::ios::in);
		m_wfin.seekg(m_current_position, std::ios_base::beg);

		if (!m_wfin.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (std::wstring* pArr = data; pArr < data + size; ++pArr)
		{
			std::size_t data_lenght;

			m_wfin.read(reinterpret_cast<wchar_t*>(&data_lenght), sizeof(data_lenght) / sizeof(wchar_t));
			auto buffer = std::make_unique<wchar_t[]>(data_lenght);

			m_wfin.read(buffer.get(), data_lenght);
			*pArr = buffer.get();
		}

		m_current_position = m_wfin.tellg();
		m_wfin.close();
	}

	template<>
	void SerializationBinary::SerializeArrayOfData<wchar_t>(wchar_t* data, const int size, std::string_view path)
	{
		m_wfout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (!m_wfout.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (wchar_t* pArr = data; pArr < data + size; ++pArr)
		{
			m_wfout.write(reinterpret_cast<const wchar_t*>(pArr), sizeof(wchar_t) / sizeof(wchar_t));
		}

		m_wfout.close();
	}

	template<>
	void SerializationBinary::DeserializeArrayOfData<wchar_t>(wchar_t* data, const int size, std::string_view path)
	{
		m_wfin.open(path.data(), std::ios::binary | std::ios::in);
		m_wfin.seekg(m_current_position, std::ios_base::beg);

		if (!m_wfin.is_open())
		{
			throw std::exception("error opening the file");
		}

		for (wchar_t* pArr = data; pArr < data + size; ++pArr)
		{
			m_wfin.read(reinterpret_cast<wchar_t*>(pArr), sizeof(wchar_t) / sizeof(wchar_t));
		}

		m_current_position = m_wfin.tellg();
		m_wfin.close();
	}

#pragma endregion

#pragma region part with one data type

	template<typename T>
	void SerializationBinary::SerializeData(const T& data, std::string_view path)
	{
		m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}

		m_fout.write(reinterpret_cast<const char*>(&data), sizeof(T));
		m_fout.close();
	}

	template<typename T>
	void SerializationBinary::DeserializeData(T& data, std::string_view path)
	{
		m_fin.open(path.data(), std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}

		m_fin.read(reinterpret_cast<char*>(&data), sizeof(T));
		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	template<>
	void SerializationBinary::SerializeData<std::string>(const std::string& data, std::string_view path)
	{
		m_fout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (!m_fout.is_open())
		{
			throw std::exception("error opening the file");
		}

		std::size_t data_lenght = data.length() + 1;

		m_fout.write(reinterpret_cast<const char*>(&data_lenght), sizeof(data_lenght));
		m_fout.write(reinterpret_cast<const char*>(data.c_str()), data_lenght);

		m_fout.close();
	}

	template<>
	void SerializationBinary::DeserializeData<std::string>(std::string& data, std::string_view path)
	{
		m_fin.open(path.data(), std::ios::binary | std::ios::in);
		m_fin.seekg(m_current_position, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}

		std::size_t data_lenght;

		m_fin.read(reinterpret_cast<char*>(&data_lenght), sizeof(data_lenght));
		auto buffer = std::make_unique<char[]>(data_lenght);

		m_fin.read(buffer.get(), data_lenght);
		data = buffer.get();

		m_current_position = m_fin.tellg();
		m_fin.close();
	}

	template<>
	void SerializationBinary::SerializeData<std::wstring>(const std::wstring& data, std::string_view path)
	{
		m_wfout.open(path.data(), std::ios::binary | std::ios::out | std::ios::app);

		if (!m_wfout.is_open())
		{
			throw std::exception("error opening the file");
		}

		std::size_t data_lenght = data.length() + 1;

		m_wfout.write(reinterpret_cast<const wchar_t*>(&data_lenght), sizeof(data_lenght) / sizeof(wchar_t));
		m_wfout.write(reinterpret_cast<const wchar_t*>(data.c_str()), data_lenght);

		m_wfout.close();
	}

	template<>
	void SerializationBinary::DeserializeData<std::wstring>(std::wstring& data, std::string_view path)
	{
		m_wfin.open(path.data(), std::ios::in | std::ios::binary);
		m_wfin.seekg(m_current_position, std::ios_base::beg);

		if (!m_wfin.is_open())
		{
			throw std::exception("error opening the file");
		}

		std::size_t data_lenght;

		m_wfin.read(reinterpret_cast<wchar_t*>(&data_lenght), sizeof(data_lenght) / sizeof(wchar_t));
		auto buffer = std::make_unique<wchar_t[]>(data_lenght);

		m_wfin.read(buffer.get(), data_lenght);
		data = buffer.get();

		m_current_position = m_wfin.tellg();
		m_wfin.close();
	}

	template<>
	void SerializationBinary::SerializeData<wchar_t>(const wchar_t& data, std::string_view path)
	{
		m_wfout.open(path.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (!m_wfout.is_open())
		{
			throw std::exception("error opening the file");
		}

		m_wfout.write(reinterpret_cast<const wchar_t*>(&data), sizeof(wchar_t) / sizeof(wchar_t));
		m_wfout.close();
	}

	template<>
	void SerializationBinary::DeserializeData<wchar_t>(wchar_t& data, std::string_view path)
	{
		m_wfin.open(path.data(), std::ios::binary | std::ios::in);
		m_wfin.seekg(m_current_position, std::ios_base::beg);

		if (!m_wfin.is_open())
		{
			throw std::exception("error opening the file");
		}

		m_wfin.read(reinterpret_cast<wchar_t*>(&data), sizeof(wchar_t) / sizeof(wchar_t));
		m_current_position = m_wfin.tellg();
		m_wfin.close();
	}

#pragma endregion

#pragma region other

	bool SerializationBinary::IsFileEmpty(std::string_view path)
	{
		m_fin.open(path.data(), std::ios::binary | std::ios::in);
		m_fin.seekg(NULL, std::ios_base::beg);

		if (!m_fin.is_open())
		{
			throw std::exception("error opening the file");
		}

		bool result = m_fin.peek() == std::ifstream::traits_type::eof();
		m_fin.close();

		return result;
	}

	void SerializationBinary::ResetFileReadPosition() { m_current_position = NULL; }

	void SerializationBinary::ClearFile(std::string_view path)
	{
		m_current_position = NULL;
		m_fout.open(path.data(), std::ios::out | std::ios::trunc);
		m_fout.close();
	}

#pragma endregion

#pragma endregion
}

#endif