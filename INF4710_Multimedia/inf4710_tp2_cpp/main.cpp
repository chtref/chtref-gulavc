
// INF4710 A2016 TP2 v1

#include "tp2.h"

// one function per header to simplify grading (bad habit though)
#include "conv_rgb2ycbcr.h"
#include "conv_ycbcr2rgb.h"
#include "dct.h"
#include "dct_inv.h"
#include "decoup.h"
#include "decoup_inv.h"
#include "huff.h"
#include "huff_inv.h"
#include "quantif.h"
#include "quantif_inv.h"
#include "zigzag.h"
#include "zigzag_inv.h"

#define USE_SUBSAMPLING 1
#define USE_QUANT_QUALITY 100

int main(int /*argc*/, char** /*argv*/) {
    try {
        const std::vector<std::string> vsTestImagePaths = {
            {"data/airplane.png"},
            {"data/baboon.png"},
            {"data/cameraman.tif"},
            {"data/lena.png"},
            {"data/logo.tif"},
            {"data/logo_noise.tif"},
            {"data/peppers.png"},
			{"data/trump.png"}
        };
        for(const std::string& sTestImagePath : vsTestImagePaths) {
            const cv::Mat oInput = cv::imread(sTestImagePath);
            if(oInput.empty() || oInput.type()!=CV_8UC3)
                CV_Error_(-1,("Could not load image at '%s', check local paths",sTestImagePath.c_str()));

/////////////////
// COMPRESSION //
/////////////////


			//Conv R-G-B to Y-Cb-Cr
            cv::Mat_<uchar> Y,Cb,Cr;
            conv_rgb2ycbcr(oInput,USE_SUBSAMPLING,Y,Cb,Cr);
	        const std::vector<cv::Mat_<uchar>> vBlocks_Y = decoup(Y);
            const std::vector<cv::Mat_<uchar>> vBlocks_Cb = decoup(Cb);
            const std::vector<cv::Mat_<uchar>> vBlocks_Cr = decoup(Cr);

            std::vector<cv::Mat_<uchar>> vBlocks;
            vBlocks.insert(vBlocks.end(),vBlocks_Y.begin(),vBlocks_Y.end());
            vBlocks.insert(vBlocks.end(),vBlocks_Cb.begin(),vBlocks_Cb.end());
            vBlocks.insert(vBlocks.end(),vBlocks_Cr.begin(),vBlocks_Cr.end());

			// DCT
            std::vector<cv::Mat_<float>> vDCTBlocks(vBlocks.size());
            for(size_t b=0; b<vBlocks.size(); ++b)
                vDCTBlocks[b] = dct(vBlocks[b]);
/*
			// Quantif
            std::vector<cv::Mat_<short>> vQuantifDCTBlocks(vDCTBlocks.size());
            for(size_t b=0; b<vDCTBlocks.size(); ++b)
                vQuantifDCTBlocks[b] = quantif(vDCTBlocks[b],USE_QUANT_QUALITY);
            std::vector<std::array<short,8*8>> vInlinedBlocks(vQuantifDCTBlocks.size());
            for(size_t b=0; b<vQuantifDCTBlocks.size(); ++b)
                vInlinedBlocks[b] = zigzag(vQuantifDCTBlocks[b]);
            
			// Huff
			const HuffOutput<short> oCode = huff(vInlinedBlocks);

            // Calcul du taux de compression
			// TODO: Taux de Compr. = 1 - (Longueur du signal compressť / Longueur du signal original)
			//oCode.string.size() = taille du vecteur de bool (en bits) // oInput.cols * oInput.rows * oInput.channels() taille de la matrice incluant le channelname en uchar (8 bits)
			std::cout << "img: " << sTestImagePath << " -- taux: " << 1 - (double(oCode.string.size()) / double((oInput.cols * oInput.rows * oInput.channels()) * 8)) << std::endl;


///////////////////
// DECOMPRESSION //
///////////////////


			// Huff INV
            const std::vector<std::array<short,8*8>> vInlinedBlocks_decompr = huff_inv<8*8>(oCode);
            std::vector<cv::Mat_<short>> vQuantifDCTBlocks_decompr(vInlinedBlocks_decompr.size());
            for(size_t b=0; b<vInlinedBlocks_decompr.size(); ++b)
                vQuantifDCTBlocks_decompr[b] = zigzag_inv(vInlinedBlocks_decompr[b]);

			// Quantif INV
            std::vector<cv::Mat_<float>> vDCTBlocks_decompr(vQuantifDCTBlocks_decompr.size());
            for(size_t b=0; b<vQuantifDCTBlocks_decompr.size(); ++b)
                vDCTBlocks_decompr[b] = quantif_inv(vQuantifDCTBlocks_decompr[b],USE_QUANT_QUALITY);
*/
			// DCT INV
			std::vector<cv::Mat_<float>> vDCTBlocks_decompr = vDCTBlocks;

            std::vector<cv::Mat_<uchar>> vBlocks_decompr(vDCTBlocks_decompr.size());
            for(size_t b=0; b<vDCTBlocks_decompr.size(); ++b)
                vBlocks_decompr[b] = dct_inv(vDCTBlocks_decompr[b]);

			// Conv Y-Cb-Cr to R-G-B			
            const std::vector<cv::Mat_<uchar>> vBlocks_Y_decompr(vBlocks_decompr.begin(),vBlocks_decompr.begin()+vBlocks_Y.size());
            const std::vector<cv::Mat_<uchar>> vBlocks_Cb_decompr(vBlocks_decompr.begin()+vBlocks_Y.size(),vBlocks_decompr.begin()+vBlocks_Y.size()+vBlocks_Cb.size());
            const std::vector<cv::Mat_<uchar>> vBlocks_Cr_decompr(vBlocks_decompr.begin()+vBlocks_Y.size()+vBlocks_Cb.size(),vBlocks_decompr.end());
            const cv::Mat_<uchar> Y_decompr = decoup_inv(vBlocks_Y_decompr,Y.size());
            const cv::Mat_<uchar> Cb_decompr = decoup_inv(vBlocks_Cb_decompr,Cb.size());
            const cv::Mat_<uchar> Cr_decompr = decoup_inv(vBlocks_Cr_decompr,Cr.size());
            cv::Mat oInput_decompr;
            conv_ycbcr2rgb(Y,Cb,Cr,USE_SUBSAMPLING,oInput_decompr);
			
			// Resultats
            cv::Mat oDisplay;
            cv::hconcat(oInput,oInput_decompr,oDisplay);
            cv::Mat oDiff;
            cv::absdiff(oInput,oInput_decompr,oDiff);
            cv::hconcat(oDisplay,oDiff,oDisplay);
            cv::imshow(sTestImagePath.substr(sTestImagePath.find_last_of("/\\")+1),oDisplay);
            cv::waitKey(1);

        }
        std::cout << "all done; press any key on a window to quit..." << std::endl;
        cv::waitKey(0);
        return 0;
    }
    catch(const cv::Exception& e) {
        std::cerr << "Caught cv::Exceptions: " << e.what() << std::endl;
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Caught std::runtime_error: " << e.what() << std::endl;
    }
    catch(...) {
        std::cerr << "Caught unhandled exception." << std::endl;
    }
    return 1;
}
