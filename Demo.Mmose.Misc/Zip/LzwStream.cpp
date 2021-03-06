﻿#include "StdAfx.h"
#include "LzwStream.h"

#include <vcclr.h>

#pragma managed(push, off)

#include <tchar.h>

#include "ChilkatVC9/include/CkCompression.h"
#include "ChilkatVC9/include/CkByteData.h"

#pragma managed(pop)

namespace Demo
{
	namespace Mmose
	{
		namespace Zip
		{

			LzwStream::LzwStream( System::IO::Stream^ streamData,  System::IO::Compression::CompressionMode compressionMode )
			{
			}

			LzwStream::LzwStream( System::IO::Stream^ streamData,  System::IO::Compression::CompressionMode compressionMode, System::Boolean leaveOpen )
			{
			}

			LzwStream::~LzwStream(void)
			{
				if (m_LeaveOpen == true && m_StreamData != nullptr)
					m_StreamData->Close();

				m_StreamData = nullptr;
			}

			LzwStream::!LzwStream(void)
			{
				if (m_LeaveOpen == true && m_StreamData != nullptr)
					m_StreamData->Close();

				m_StreamData = nullptr;
			}

			void LzwStream::Flush()
			{
				throw gcnew System::NotSupportedException();
			}

			System::Int32 LzwStream::Read( array<System::Byte>^ buffer, System::Int32 offset, System::Int32 count )
			{
				if (m_CompressionMode != System::IO::Compression::CompressionMode::Decompress)
					throw gcnew System::InvalidOperationException("LzwStream::Read(...) - m_CompressionMode != System::IO::Compression::CompressionMode::Decompress error!");

				if (buffer == nullptr)
					throw gcnew System::ArgumentNullException("LzwStream::Read(...) - buffer == nullptr error!");
				if (offset < 0)
					throw gcnew System::ArgumentOutOfRangeException("LzwStream::Read(...) - offset < 0 error!");
				if (count < 0)
					throw gcnew System::ArgumentOutOfRangeException("LzwStream::Read(...) - count < 0 error!");
				if ((buffer->Length - offset) < count)
					throw gcnew System::ArgumentException("LzwStream::Read(...) - (buffer->Length - offset) < count error!");
				if (m_StreamData == nullptr)
					throw gcnew System::ObjectDisposedException("m_StreamData", "LzwStream::Read(...) - m_StreamData == nullptr error!");

				int iRealSize = buffer->Length - offset;

				if (m_ResidualBuffer->Length > 0 && m_ResidualRealSize > 0)
				{
					if (m_ResidualBuffer->Length < m_ResidualRealSize)
						throw gcnew System::InvalidOperationException("LzwStream::Read(...) - m_ResidualBuffer->Length < m_ResidualRealSize error!");

					if (m_ResidualRealSize <= iRealSize)
					{
						int iResidualRealSize = m_ResidualRealSize;

						System::Buffer::BlockCopy(m_ResidualBuffer, 0, buffer, offset, iResidualRealSize);
						m_ResidualRealSize = 0;

						return iResidualRealSize;
					}
					else
					{
						System::Buffer::BlockCopy(m_ResidualBuffer, 0, buffer, offset, iRealSize);

						int iResidualSize = m_ResidualRealSize - iRealSize;
						int iResidualOffsetSrc = iRealSize;
						int iResidualOffsetDst = 0;

						while (iResidualSize > 0)
						{
							if (iResidualSize <= iRealSize)
							{
								System::Buffer::BlockCopy(m_ResidualBuffer, iResidualOffsetSrc, m_ResidualBuffer, iResidualOffsetDst, iResidualSize);

								iResidualSize = 0;
							}
							else
							{
								System::Buffer::BlockCopy(m_ResidualBuffer, iResidualOffsetSrc, m_ResidualBuffer, iResidualOffsetDst, iRealSize);

								iResidualSize -= iRealSize;
								iResidualOffsetSrc += iRealSize;
								iResidualOffsetDst += iRealSize;
							}
						}

						return iRealSize;
					}
				}

				if (m_Buffer->Length <= 0 )
					m_Buffer = gcnew array<System::Byte>(::Global::BUFFER_SIZE);

				int iLength = m_StreamData->Read(m_Buffer, 0, ::Global::BUFFER_SIZE);
				if (iLength <= 0)
					return 0;


				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::Decompress(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				pin_ptr<const unsigned char> bytePinPtr = &m_Buffer[0];

				CkByteData byteCompress;
				byteCompress.append(bytePinPtr, iLength);

				CkByteData byteDeflated;


				bool bIsDeflate = compress.DecompressBytes(byteCompress, byteDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::Decompress(...) - compress.DecompressBytes() == false error!" );


				int iDeflatedSize = (int)byteDeflated.getSize();
				if (iRealSize >= iDeflatedSize )
				{
					System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)byteDeflated.getBytes(), buffer, offset, iDeflatedSize);

					return iDeflatedSize;
				}
				else
				{
					System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)byteDeflated.getBytes(), buffer, offset, iRealSize);

					 int iResidualSize = iDeflatedSize - iRealSize;
					 if (m_ResidualBuffer->Length < iResidualSize)
						 m_ResidualBuffer = gcnew array<System::Byte>(iResidualSize);

					System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)(byteDeflated.getBytes() + iRealSize), m_ResidualBuffer, 0, iResidualSize);

					m_ResidualRealSize = iResidualSize;

					return iRealSize;
				}
			}

			System::Int64 LzwStream::Seek( System::Int64 offset, System::IO::SeekOrigin origin )
			{
				throw gcnew System::NotSupportedException();
			}

			void LzwStream::SetLength( System::Int64 value )
			{
				throw gcnew System::NotSupportedException();
			}

			void LzwStream::Write( array<System::Byte>^ buffer, System::Int32 offset, System::Int32 count )
			{
				if (m_CompressionMode != System::IO::Compression::CompressionMode::Compress)
					throw gcnew System::InvalidOperationException("LzwStream::Read(...) - m_CompressionMode != System::IO::Compression::CompressionMode::Decompress error!");

				if (buffer == nullptr)
					throw gcnew System::ArgumentNullException("LzwStream::Read(...) - buffer == nullptr error!");
				if (offset < 0)
					throw gcnew System::ArgumentOutOfRangeException("LzwStream::Read(...) - offset < 0 error!");
				if (count < 0)
					throw gcnew System::ArgumentOutOfRangeException("LzwStream::Read(...) - count < 0 error!");
				if ((buffer->Length - offset) < count)
					throw gcnew System::ArgumentException("LzwStream::Read(...) - (buffer->Length - offset) < count error!");
				if (m_StreamData == nullptr)
					throw gcnew System::ObjectDisposedException("m_StreamData", "LzwStream::Read(...) - m_StreamData == nullptr error!");

				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::Compress(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				pin_ptr<const unsigned char> bytePinPtr = &buffer[0];

				CkByteData byteCompress;
				byteCompress.append(bytePinPtr + offset, count);

				CkByteData byteDeflated;

				bool bIsDeflate = compress.CompressBytes(byteCompress, byteDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::CompressString(...) - compress.CompressString() == false error!" );

				int iDeflatedSize = (int)byteDeflated.getSize();

				if (m_Buffer->Length <= iDeflatedSize)
					m_Buffer = gcnew array<System::Byte>(iDeflatedSize);

				System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)byteDeflated.getBytes(), m_Buffer, 0, iDeflatedSize);

				m_StreamData->Write(m_Buffer, 0, iDeflatedSize);
			}



			System::String^ LzwStream::CompressString( System::String^ strData,  EncodingMode resultEncodingMode )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::CompressString(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				switch(resultEncodingMode)
				{
					case EncodingMode::Base64:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_BASE64);
						break;
					case EncodingMode::Hex:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_HEX);
						break;
					case EncodingMode::Url:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_URL);
						break;
					case EncodingMode::QuotedPrintable:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_QUOTED_PRINTABLE);
						break;
					default:
						break;
				}

				pin_ptr<const wchar_t> strPinPtr = ::PtrToStringChars(strData);

				CkString strCompress;
				strCompress.appendU(strPinPtr);

				CkString strDeflated;

				bool bIsDeflate = compress.CompressStringENC(strCompress, strDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::CompressString(...) - compress.CompressStringENC() == false error!" );

				return System::Runtime::InteropServices::Marshal::PtrToStringUni((System::IntPtr)(void*)strDeflated.getUnicode());
			}

			array<System::Byte>^ LzwStream::CompressString( System::String^ strData )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
				{
					throw gcnew System::Exception( "LzwStream::CompressString(...) - compress.UnlockComponent() == false error!" );
					return nullptr;
				}

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				pin_ptr<const wchar_t> strPinPtr = ::PtrToStringChars(strData);

				CkString strCompress;
				strCompress.appendU(strPinPtr);

				CkByteData byteDeflated;

				bool bIsDeflate = compress.CompressString(strCompress, byteDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::CompressString(...) - compress.CompressString() == false error!" );

				array<System::Byte>^ byteArray = gcnew array<System::Byte>(byteDeflated.getSize());
				System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)byteDeflated.getBytes(), byteArray, 0, byteDeflated.getSize());

				return byteArray;
			}

			System::String^ LzwStream::Compress( array<System::Byte>^ byteData, EncodingMode resultEncodingMode )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::Compress(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				switch(resultEncodingMode)
				{
					case EncodingMode::Base64:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_BASE64);
						break;
					case EncodingMode::Hex:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_HEX);
						break;
					case EncodingMode::Url:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_URL);
						break;
					case EncodingMode::QuotedPrintable:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_QUOTED_PRINTABLE);
						break;
					default:
						break;
				}

				pin_ptr<const unsigned char> bytePinPtr = &byteData[0];

				CkByteData byteCompress;
				byteCompress.append(bytePinPtr, byteData->Length);

				CkString strDeflated;

				bool bIsDeflate = compress.CompressBytesENC(byteCompress, strDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::CompressString(...) - compress.CompressStringENC() == false error!" );

				return System::Runtime::InteropServices::Marshal::PtrToStringUni((System::IntPtr)(void*)strDeflated.getUnicode());
			}

			array<System::Byte>^ LzwStream::Compress( array<System::Byte>^ byteData )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::Compress(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				pin_ptr<const unsigned char> bytePinPtr = &byteData[0];

				CkByteData byteCompress;
				byteCompress.append(bytePinPtr, byteData->Length);

				CkByteData byteDeflated;

				bool bIsDeflate = compress.CompressBytes(byteCompress, byteDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::CompressString(...) - compress.CompressString() == false error!" );

				array<System::Byte>^ byteArray = gcnew array<System::Byte>(byteDeflated.getSize());
				System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)byteDeflated.getBytes(), byteArray, 0, byteDeflated.getSize());

				return byteArray;
			}

			System::String^ LzwStream::DecompressString( System::String^ strData, EncodingMode encodingMode )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::DecompressString(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				switch(encodingMode)
				{
					case EncodingMode::Base64:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_BASE64);
						break;
					case EncodingMode::Hex:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_HEX);
						break;
					case EncodingMode::Url:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_URL);
						break;
					case EncodingMode::QuotedPrintable:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_QUOTED_PRINTABLE);
						break;
					default:
						break;
				}

				pin_ptr<const wchar_t> strPinPtr = ::PtrToStringChars(strData);

				CkString strCompress;
				strCompress.appendU(strPinPtr);

				CkString strDeflated;

				bool bIsDeflate = compress.DecompressStringENC(strCompress, strDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::DecompressString(...) - compress.CompressStringENC() == false error!" );

				return System::Runtime::InteropServices::Marshal::PtrToStringUni((System::IntPtr)(void*)strDeflated.getUnicode());
			}

			System::String^ LzwStream::DecompressString( array<System::Byte>^ byteData )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::DecompressString(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				pin_ptr<const unsigned char> bytePinPtr = &byteData[0];

				CkByteData byteCompress;
				byteCompress.append(bytePinPtr, byteData->Length);

				CkString strDeflated;

				bool bIsDeflate = compress.DecompressString(byteCompress, strDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::DecompressString(...) - compress.DecompressString() == false error!" );

				return System::Runtime::InteropServices::Marshal::PtrToStringUni((System::IntPtr)(void*)strDeflated.getUnicode());
			}

			array<System::Byte>^ LzwStream::Decompress( System::String^ strData, EncodingMode encodingMode )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::Decompress(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				switch(encodingMode)
				{
					case EncodingMode::Base64:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_BASE64);
						break;
					case EncodingMode::Hex:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_HEX);
						break;
					case EncodingMode::Url:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_URL);
						break;
					case EncodingMode::QuotedPrintable:
						compress.put_EncodingMode(GlobalString::COMPRESS_ENCODING_QUOTED_PRINTABLE);
						break;
					default:
						break;
				}

				pin_ptr<const wchar_t> strPinPtr = ::PtrToStringChars(strData);

				CkString strCompress;
				strCompress.appendU(strPinPtr);

				CkByteData byteDeflated;

				bool bIsDeflate = compress.DecompressBytesENC(strCompress, byteDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::Decompress(...) - compress.DecompressBytesENC() == false error!" );

				array<System::Byte>^ byteArray = gcnew array<System::Byte>(byteDeflated.getSize());
				System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)byteDeflated.getBytes(), byteArray, 0, byteDeflated.getSize());

				return byteArray;
			}

			array<System::Byte>^ LzwStream::Decompress( array<System::Byte>^ byteData )
			{
				CkCompression compress;

				bool bSuccess = compress.UnlockComponent(GlobalString::COMPRESS_LICENCE);
				if (bSuccess == false)
					throw gcnew System::Exception( "LzwStream::Decompress(...) - compress.UnlockComponent() == false error!" );

				compress.put_Algorithm(GlobalString::COMPRESS_ALGORITHM_LZW);
				compress.put_Charset(GlobalString::COMPRESS_CHAR_SET);

				pin_ptr<const unsigned char> bytePinPtr = &byteData[0];

				CkByteData byteCompress;
				byteCompress.append(bytePinPtr, byteData->Length);

				CkByteData byteDeflated;

				bool bIsDeflate = compress.DecompressBytes(byteCompress, byteDeflated);
				if (bIsDeflate == false )
					throw gcnew System::Exception( "LzwStream::Decompress(...) - compress.DecompressBytes() == false error!" );

				array<System::Byte>^ byteArray = gcnew array<System::Byte>(byteDeflated.getSize());
				System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)(void*)byteDeflated.getBytes(), byteArray, 0, byteDeflated.getSize());

				return byteArray;
			}

		}
	}
}