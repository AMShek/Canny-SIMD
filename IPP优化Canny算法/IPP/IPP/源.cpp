/*
// Copyright 2015 2016 Intel Corporation All Rights Reserved.
//
// The source code, information and material ("Material") contained herein is
// owned by Intel Corporation or its suppliers or licensors, and title
// to such Material remains with Intel Corporation or its suppliers or
// licensors. The Material contains proprietary information of Intel
// or its suppliers and licensors. The Material is protected by worldwide
// copyright laws and treaty provisions. No part of the Material may be used,
// copied, reproduced, modified, published, uploaded, posted, transmitted,
// distributed or disclosed in any way without Intel's prior express written
// permission. No license under any patent, copyright or other intellectual
// property rights in the Material is granted to or conferred upon you,
// either expressly, by implication, inducement, estoppel or otherwise.
// Any license under such intellectual property rights must be express and
// approved by Intel in writing.
//
// Unless otherwise agreed by Intel in writing,
// you may not remove or alter this notice or any other notice embedded in
// Materials by Intel or Intel's suppliers or licensors in any way.
*/

// A simple example of the edge detection algorithm implemented with Intel IPP functions:
//     ippiFilterSobelNegVertBorder_8u16s_C1R
//     ippiFilterSobelHorizBorder_8u16s_C1R
//     ippiCanny_16s8u_C1R


#include <stdio.h>
#include<Windows.h>
#include "imageio.h"
#include "ipp.h"

#define THRESH_LOW    50.f /* Low   threshold for edges detection */
#define THRESH_HIGHT 150.f /* Upper threshold for edges detection */
#define BORDER_VAL 0

/* Next two defines are created to simplify code reading and understanding */
#define EXIT_MAIN exitLine:                                  /* Label for Exit */
#define check_sts(st) if((st) != ippStsNoErr) goto exitLine; /* Go to Exit if IPP function returned status different from ippStsNoErr */

/* Results of ippMalloc() are not validated because IPP functions perform bad arguments check and will return an appropriate status  */

int main(int argc, char *argv[])
{
	for (int counter=0; counter<100; counter++){
		FILE *fp;
		int w, h, i;
		h = HEIGHT;
		w = WIDTH;
		LARGE_INTEGER t1, t2, tc;
		QueryPerformanceFrequency(&tc);
		if ((fp = fopen(argv[1], "r")) == NULL && argc == 1) {
			printf("ERROR: %s can't open %s!", argv[0], argv[1]);
		}
		else {
			if (read_pgm_hdr(fp, &w, &h) != -1) {
				//printf("*** PGM file recognized, reading data into image struct ***\n");
				IppiSize roiSize = { WIDTH, HEIGHT }; /* Size of source/destination ROI in pixels */
				IppStatus status = ippStsNoErr;
				Ipp8u* pSrc = NULL, *pDst = NULL;  /* Pointers to source/destination images */
				int srcStep = 0, dstStep = 0;      /* Steps, in bytes, through the source/destination images */
				pSrc = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &srcStep);
				pDst = ippiMalloc_8u_C1(roiSize.width, roiSize.height, &dstStep);


				for (i = 0; i < w * h; i++) {//reading PGM file into image struct
					pSrc[i] = fgetc(fp);
				}
				//printf("FINISH READING node!!!!!!!\n");

				int iTmpBufSizeSobV = 0;
				int iTmpBufSizeSobH = 0;
				int iTmpBufSizeCanny = 0;
				int iTmpBufSize = 0;         /* Common work buffer size */
				int  dxStep = 0, dyStep = 0; /* Steps, in bytes, through buffer_dx/buffer_dy */
				Ipp8u *buffer = NULL;        /* Pointer to the common work buffer */
				Ipp16s* buffer_dx = NULL, *buffer_dy = NULL; /* Pointer to the buffer for first derivatives with respect to X / Y */

				/* Start Edge Detection algorithm */
				{
				QueryPerformanceCounter(&t1);//start time recording!
				buffer_dx = ippiMalloc_16s_C1(roiSize.width, roiSize.height, &dxStep);
				buffer_dy = ippiMalloc_16s_C1(roiSize.width, roiSize.height, &dyStep);

				check_sts(status = ippiFilterSobelVertBorderGetBufferSize(roiSize, ippMskSize3x3, ipp8u, ipp16s, 1, &iTmpBufSizeSobV))
					check_sts(status = ippiFilterSobelHorizBorderGetBufferSize(roiSize, ippMskSize3x3, ipp8u, ipp16s, 1, &iTmpBufSizeSobH))

					check_sts(ippiCannyGetSize(roiSize, &iTmpBufSizeCanny))

					/* Find maximum buffer size */
					//printf("Find maximum buffer size node!!!!!!!\n");
					iTmpBufSize = (iTmpBufSizeSobV > iTmpBufSizeSobH) ? iTmpBufSizeSobV : iTmpBufSizeSobH;
				iTmpBufSize = (iTmpBufSize > iTmpBufSizeCanny) ? iTmpBufSize : iTmpBufSizeCanny;
				buffer = ippsMalloc_8u(iTmpBufSize);

				check_sts(status = ippiFilterSobelNegVertBorder_8u16s_C1R(pSrc, srcStep, buffer_dx, dxStep, roiSize, ippMskSize3x3, ippBorderRepl, BORDER_VAL, buffer))

					check_sts(status = ippiFilterSobelHorizBorder_8u16s_C1R(pSrc, srcStep, buffer_dy, dyStep, roiSize, ippMskSize3x3, ippBorderRepl, BORDER_VAL, buffer))

					check_sts(status = ippiCanny_16s8u_C1R(buffer_dx, dxStep, buffer_dy, dyStep, pDst, dstStep, roiSize, THRESH_LOW, THRESH_HIGHT, buffer))
					QueryPerformanceCounter(&t2);//stop time recording!!
				printf("%f\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);
				}

				write_pgm_image(pDst);

				EXIT_MAIN
			    ippsFree(buffer);
				ippiFree(buffer_dx);
				ippiFree(buffer_dy);
				ippiFree(pSrc);
				ippiFree(pDst);
				//printf("Exit status %d (%s)\n", (int)status, ippGetStatusString(status));
				//system("pause");
				/*ippsFree(buffer);
				ippiFree(buffer_dx);
				ippiFree(buffer_dy);
				ippiFree(pSrc);
				ippiFree(pDst);*/
				//system("pause");
				//return (int)status;
			}
			else {
				continue;
			}

		}
	}
	return 0;
}
