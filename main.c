#include <CL/cl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
const uint8_t radix_table[128] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 31, 63, 255,
	0, 32, 5, 37, 10, 42, 17, 49, 24, 56, 255, 255, 255, 255, 255, 255,
	255, 1, 2, 3, 4, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 18,
	19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 255, 255, 255, 255, 255,
	255, 33, 34, 35, 36, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 50,
	51, 52, 53, 54, 55, 57, 58, 59, 60, 61, 62, 255, 255, 255, 255, 255,
};

template <typename Callback>
			static void decodeCoords(const std::string &radix64, Callback callback)
			{
				decodeCoords<std::string::value_type, Callback>(radix64.data(), radix64.length(), callback);
			}

			template <typename DataType, typename Callback>
			static void decodeCoords(const DataType* radix64, size_t length, Callback callback)
			{
				int32_t x = 0;
				int32_t y = 0;
				double lon, lat;

				for (size_t index = 0; index < length;)
				{
					// encoded string may contain spaces - remove them
					if (radix64[index] == ' ')
					{
						++index;
						continue;
					}

					x = decodeCoord<DataType>(radix64, index, x);
					y = decodeCoord<DataType>(radix64, index, y);

					lon = (double)x * (360.0 / (1 << 28)) - 180.0;
					lat = (double)y * (180.0 / (1 << 28)) - 90.0;

					callback(lon, lat);
				}
			}


			template <typename DataType>
			static int32_t decodeCoord(const DataType* radix64, size_t &index, int32_t prevCoord)
			{
				int result = 0;

				uint8_t w0 = m_Radix64TableReverse[(uint8_t)radix64[index++]];

				// Decode length of encoded integer base on upper bits of w0
				if ((w0 & 0x20) == 0)
				{
					// 3 words (0XXXXX XXXXXX)
					// difference encoded
					result = w0;
					result = (result << 6) + m_Radix64TableReverse[(uint8_t)radix64[index++]];
					return prevCoord + result - 1024;
				}
				else if ((w0 & 0x10) == 0)
				{
					// 3 words (10XXXX XXXXXX XXXXXX)
					// difference encoded
					result = w0 & 0xF;
					result = (result << 6) + m_Radix64TableReverse[(uint8_t)radix64[index++]];
					result = (result << 6) + m_Radix64TableReverse[(uint8_t)radix64[index++]];
					return prevCoord + result - 32768;
				}
				else
				{
					// 5 words (11XXXX XXXXXX XXXXXX XXXXXX XXXXXX)
					// coordinate encoded
					result = w0 & 0xF;
					result = (result << 6) + m_Radix64TableReverse[(uint8_t)radix64[index++]];
					result = (result << 6) + m_Radix64TableReverse[(uint8_t)radix64[index++]];
					result = (result << 6) + m_Radix64TableReverse[(uint8_t)radix64[index++]];
					result = (result << 6) + m_Radix64TableReverse[(uint8_t)radix64[index++]];
					return result;
				}
			}


*/

cl_platform_id select_platform();

cl_device_id select_device(cl_platform_id);

int main(int argc, char *argv[]) {

	int err;

	cl_platform_id plat = select_platform();

	cl_device_id dev = select_device(plat);
	cl_context_properties ctxprop[3] = {
		(cl_context_properties)CL_CONTEXT_PLATFORM,
		(cl_context_properties)plat,
		(cl_context_properties)0
	};

	cl_context ctx = clCreateContext(ctxprop,1,&dev,0,0,&err);
	cl_command_queue cmdq = clCreateCommandQueue(ctx,dev,0,&err);





	clReleaseCommandQueue(cmdq);
	clReleaseContext(ctx);

	return 0;
}


cl_platform_id select_platform() {

	cl_uint nplatforms;
	cl_platform_id* platforms;
	cl_platform_id platform = 0;

	clGetPlatformIDs( 0,0,&nplatforms);
	platforms = (cl_platform_id*)malloc(nplatforms*sizeof(cl_platform_id));
	clGetPlatformIDs( nplatforms, platforms, 0);

	char buffer[256];
	for(int i=0; i<nplatforms; i++) {
		platform = platforms[i];
		clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,256,buffer,0);
		if (!strcmp(buffer,"coprthr-e")) break;
	}

	return platform;
}

cl_device_id select_device(cl_platform_id plat) {

	cl_uint ndev;
	cl_device_id *devs;

	clGetDeviceIDs(plat, CL_DEVICE_TYPE_ACCELERATOR, 0, 0, &ndev);
	devs = (cl_device_id*)malloc(ndev * sizeof(cl_device_id));
	clGetDeviceIDs(plat, CL_DEVICE_TYPE_ACCELERATOR, ndev, devs, 0);

	return (ndev > 0) ? (devs[0]) : (cl_device_id)0;
}
