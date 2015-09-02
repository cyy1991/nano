#include "synth_digits.h"

ncv::rgba_matrix_t ncv::get_synth_digits()
{
        rgba_matrix_t data(32, 175);

        data.row(0) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        data.row(1) << 
        0, 0, 0, 0, 0, 0, 15, 77, 107, 96, 57, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 92, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 70, 93, 92, 53, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 28, 66, 99, 75, 39, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 85, 92, 26, 0, 0, 0, 0, 0, 0, 0, 104, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 84, 107, 73, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 63, 102, 87, 47, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 81, 102, 85, 48, 1, 0, 0, 0, 0, 0, 0;

        data.row(2) << 
        0, 0, 0, 0, 17, 151, 248, 255, 255, 255, 255, 227, 91, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 252, 255, 28, 0, 0, 0, 0, 0, 0, 0, 0, 43, 182, 252, 255, 255, 255, 255, 255, 190, 54, 0, 0, 0, 0, 0, 0, 0, 4, 119, 233, 255, 255, 255, 255, 255, 241, 121, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 255, 255, 72, 0, 0, 0, 0, 0, 0, 0, 169, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 60, 0, 0, 0, 0, 0, 0, 0, 64, 200, 255, 255, 255, 255, 255, 175, 33, 0, 0, 0, 0, 136, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 0, 0, 0, 0, 42, 182, 255, 255, 255, 255, 255, 244, 124, 10, 0, 0, 0, 0, 0, 0, 0, 0, 65, 198, 255, 255, 255, 255, 255, 222, 93, 1, 0, 0, 0, 0;

        data.row(3) << 
        0, 0, 0, 25, 216, 255, 255, 255, 255, 255, 255, 255, 255, 137, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 114, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 85, 250, 255, 255, 255, 255, 255, 255, 255, 255, 251, 94, 0, 0, 0, 0, 0, 4, 182, 255, 255, 255, 255, 255, 255, 255, 255, 255, 222, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 250, 255, 255, 72, 0, 0, 0, 0, 0, 0, 0, 204, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 60, 0, 0, 0, 0, 0, 0, 121, 254, 255, 255, 255, 255, 255, 255, 255, 244, 41, 0, 0, 0, 136, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 0, 0, 0, 100, 252, 255, 255, 255, 255, 255, 255, 255, 255, 221, 28, 0, 0, 0, 0, 0, 0, 121, 255, 255, 255, 255, 255, 255, 255, 255, 255, 153, 1, 0, 0, 0;

        data.row(4) << 
        0, 0, 1, 186, 255, 255, 205, 67, 12, 26, 114, 248, 255, 255, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 245, 255, 255, 28, 0, 0, 0, 0, 0, 0, 45, 248, 255, 255, 188, 70, 17, 24, 74, 198, 255, 255, 252, 56, 0, 0, 0, 0, 124, 255, 255, 243, 120, 37, 9, 35, 99, 229, 255, 255, 188, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 221, 255, 255, 255, 72, 0, 0, 0, 0, 0, 0, 0, 239, 255, 186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 255, 255, 241, 110, 28, 13, 57, 185, 255, 255, 212, 10, 0, 0, 102, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 215, 255, 255, 125, 0, 0, 48, 253, 255, 255, 164, 54, 14, 28, 85, 217, 255, 255, 196, 1, 0, 0, 0, 0, 92, 255, 255, 255, 150, 58, 14, 48, 121, 247, 255, 255, 113, 0, 0, 0;

        data.row(5) << 
        0, 0, 75, 255, 255, 216, 12, 0, 0, 0, 0, 78, 255, 255, 221, 4, 0, 0, 0, 0, 1, 29, 72, 156, 247, 255, 255, 255, 28, 0, 0, 0, 0, 0, 0, 162, 255, 255, 158, 2, 0, 0, 0, 0, 2, 159, 255, 255, 196, 0, 0, 0, 5, 239, 255, 247, 48, 0, 0, 0, 0, 0, 32, 243, 255, 255, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 169, 255, 255, 255, 255, 72, 0, 0, 0, 0, 0, 0, 18, 255, 255, 149, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 238, 255, 244, 44, 0, 0, 0, 0, 3, 186, 255, 255, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 197, 255, 206, 6, 0, 0, 168, 255, 255, 139, 0, 0, 0, 0, 0, 18, 228, 255, 255, 63, 0, 0, 0, 13, 235, 255, 255, 87, 0, 0, 0, 0, 0, 44, 242, 255, 240, 19, 0, 0;

        data.row(6) << 
        0, 0, 174, 255, 255, 83, 0, 0, 0, 0, 0, 0, 187, 255, 255, 68, 0, 0, 0, 0, 182, 255, 255, 255, 255, 255, 255, 255, 28, 0, 0, 0, 0, 0, 15, 250, 255, 243, 12, 0, 0, 0, 0, 0, 0, 12, 233, 255, 255, 31, 0, 0, 41, 255, 255, 160, 0, 0, 0, 0, 0, 0, 0, 147, 255, 255, 106, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 255, 255, 214, 255, 255, 72, 0, 0, 0, 0, 0, 0, 53, 255, 255, 113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 124, 255, 255, 122, 0, 0, 0, 0, 0, 0, 49, 255, 255, 171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 255, 247, 41, 0, 0, 0, 234, 255, 255, 16, 0, 0, 0, 0, 0, 0, 120, 255, 255, 129, 0, 0, 0, 96, 255, 255, 171, 0, 0, 0, 0, 0, 0, 0, 111, 255, 255, 110, 0, 0;

        data.row(7) << 
        0, 11, 247, 255, 248, 11, 0, 0, 0, 0, 0, 0, 106, 255, 255, 153, 0, 0, 0, 0, 188, 255, 255, 255, 255, 255, 255, 255, 28, 0, 0, 0, 0, 0, 49, 255, 255, 171, 0, 0, 0, 0, 0, 0, 0, 0, 174, 255, 255, 74, 0, 0, 83, 255, 255, 106, 0, 0, 0, 0, 0, 0, 0, 105, 255, 255, 125, 0, 0, 0, 0, 0, 0, 0, 0, 45, 247, 255, 155, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 88, 255, 255, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 211, 255, 254, 28, 0, 0, 0, 0, 0, 0, 1, 91, 96, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 249, 255, 107, 0, 0, 0, 1, 253, 255, 232, 0, 0, 0, 0, 0, 0, 0, 81, 255, 255, 149, 0, 0, 0, 162, 255, 255, 82, 0, 0, 0, 0, 0, 0, 0, 13, 255, 255, 193, 0, 0;

        data.row(8) << 
        0, 55, 255, 255, 181, 0, 0, 0, 0, 0, 0, 0, 30, 255, 255, 206, 0, 0, 0, 0, 15, 20, 20, 20, 20, 190, 255, 255, 28, 0, 0, 0, 0, 0, 79, 255, 255, 134, 0, 0, 0, 0, 0, 0, 0, 0, 150, 255, 255, 78, 0, 0, 68, 148, 148, 51, 0, 0, 0, 0, 0, 0, 0, 120, 255, 255, 99, 0, 0, 0, 0, 0, 0, 0, 11, 215, 255, 214, 11, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 123, 255, 255, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 255, 255, 188, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 206, 255, 182, 1, 0, 0, 0, 0, 232, 255, 255, 15, 0, 0, 0, 0, 0, 0, 117, 255, 255, 126, 0, 0, 0, 189, 255, 255, 39, 0, 0, 0, 0, 0, 0, 0, 0, 221, 255, 244, 1, 0;

        data.row(9) << 
        0, 98, 255, 255, 129, 0, 0, 0, 0, 0, 0, 0, 0, 234, 255, 245, 2, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 36, 92, 92, 41, 0, 0, 0, 0, 0, 0, 0, 0, 199, 255, 255, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 213, 255, 252, 32, 0, 0, 0, 0, 0, 0, 0, 159, 255, 247, 45, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 158, 255, 253, 18, 121, 195, 233, 246, 220, 157, 59, 0, 0, 0, 0, 0, 0, 75, 255, 255, 137, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 130, 255, 245, 27, 0, 0, 0, 0, 0, 151, 255, 255, 139, 0, 0, 0, 0, 0, 17, 225, 255, 255, 46, 0, 0, 0, 189, 255, 255, 52, 0, 0, 0, 0, 0, 0, 0, 0, 228, 255, 255, 30, 0;

        data.row(10) << 
        0, 130, 255, 255, 110, 0, 0, 0, 0, 0, 0, 0, 0, 215, 255, 255, 24, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 255, 255, 219, 3, 0, 0, 0, 0, 0, 0, 0, 0, 5, 16, 36, 95, 209, 255, 255, 132, 0, 0, 0, 0, 0, 0, 0, 93, 255, 255, 103, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 194, 255, 250, 234, 255, 255, 255, 255, 255, 255, 255, 162, 9, 0, 0, 0, 0, 115, 255, 255, 112, 1, 88, 182, 231, 250, 232, 179, 90, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 250, 255, 129, 0, 0, 0, 0, 0, 0, 20, 216, 255, 255, 162, 53, 14, 27, 85, 216, 255, 255, 132, 0, 0, 0, 0, 161, 255, 255, 95, 0, 0, 0, 0, 0, 0, 0, 23, 255, 255, 255, 51, 0;

        data.row(11) << 
        0, 142, 255, 255, 92, 0, 0, 0, 0, 0, 0, 0, 0, 196, 255, 255, 36, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, 235, 255, 255, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 236, 255, 255, 255, 255, 234, 108, 1, 0, 0, 0, 0, 0, 0, 39, 244, 255, 170, 1, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 229, 255, 255, 255, 224, 150, 129, 164, 218, 255, 255, 255, 182, 3, 0, 0, 0, 137, 255, 255, 100, 187, 255, 255, 255, 255, 255, 255, 255, 200, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 172, 255, 233, 13, 0, 0, 0, 0, 0, 0, 0, 22, 179, 255, 255, 255, 255, 255, 255, 255, 245, 105, 1, 0, 0, 0, 0, 93, 255, 255, 210, 4, 0, 0, 0, 0, 0, 0, 156, 255, 255, 255, 62, 0;

        data.row(12) << 
        0, 154, 255, 255, 74, 0, 0, 0, 0, 0, 0, 0, 0, 178, 255, 255, 47, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 112, 247, 255, 255, 156, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 236, 255, 255, 255, 255, 226, 107, 7, 0, 0, 0, 0, 0, 8, 209, 255, 223, 16, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 9, 255, 255, 255, 133, 5, 0, 0, 0, 2, 114, 249, 255, 255, 105, 0, 0, 0, 148, 255, 255, 231, 255, 255, 194, 145, 135, 182, 250, 255, 255, 215, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 59, 255, 255, 104, 0, 0, 0, 0, 0, 0, 0, 0, 14, 145, 255, 255, 255, 255, 255, 255, 255, 230, 82, 1, 0, 0, 0, 0, 9, 230, 255, 255, 159, 13, 0, 0, 0, 7, 130, 255, 255, 255, 255, 69, 0;

        data.row(13) << 
        0, 156, 255, 255, 72, 0, 0, 0, 0, 0, 0, 0, 0, 178, 255, 255, 52, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 77, 216, 255, 255, 255, 155, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 95, 123, 162, 248, 255, 255, 185, 10, 0, 0, 0, 0, 150, 255, 251, 56, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 20, 144, 144, 100, 0, 0, 0, 0, 0, 0, 0, 121, 255, 255, 225, 1, 0, 0, 158, 255, 255, 255, 227, 66, 0, 0, 0, 0, 48, 218, 255, 255, 139, 0, 0, 0, 0, 0, 0, 0, 0, 1, 201, 255, 217, 5, 0, 0, 0, 0, 0, 0, 0, 29, 223, 255, 255, 217, 139, 99, 114, 161, 249, 255, 255, 150, 1, 0, 0, 0, 0, 74, 254, 255, 255, 229, 166, 132, 165, 225, 255, 254, 230, 255, 255, 59, 0;

        data.row(14) << 
        0, 144, 255, 255, 89, 0, 0, 0, 0, 0, 0, 0, 0, 196, 255, 255, 40, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 188, 255, 255, 255, 231, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 205, 255, 255, 132, 0, 0, 0, 84, 255, 255, 118, 0, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 229, 255, 255, 45, 0, 0, 152, 255, 255, 255, 61, 0, 0, 0, 0, 0, 0, 57, 255, 255, 245, 8, 0, 0, 0, 0, 0, 0, 0, 83, 255, 255, 100, 0, 0, 0, 0, 0, 0, 0, 6, 215, 255, 255, 131, 6, 0, 0, 0, 0, 36, 207, 255, 255, 120, 0, 0, 0, 0, 0, 83, 243, 255, 255, 255, 255, 255, 255, 247, 95, 185, 255, 255, 48, 0;

        data.row(15) << 
        0, 131, 255, 255, 108, 0, 0, 0, 0, 0, 0, 0, 0, 213, 255, 255, 27, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 0, 3, 110, 245, 255, 255, 243, 128, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 254, 255, 236, 1, 0, 15, 240, 255, 185, 2, 0, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 175, 255, 255, 86, 0, 0, 141, 255, 255, 185, 0, 0, 0, 0, 0, 0, 0, 0, 193, 255, 255, 65, 0, 0, 0, 0, 0, 0, 0, 192, 255, 244, 13, 0, 0, 0, 0, 0, 0, 0, 98, 255, 255, 177, 1, 0, 0, 0, 0, 0, 0, 38, 246, 255, 239, 14, 0, 0, 0, 0, 0, 23, 132, 206, 241, 248, 222, 150, 37, 0, 210, 255, 255, 28, 0;

        data.row(16) << 
        0, 98, 255, 255, 126, 0, 0, 0, 0, 0, 0, 0, 0, 231, 255, 248, 3, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 0, 7, 183, 255, 255, 253, 147, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 219, 255, 255, 21, 0, 28, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 255, 255, 103, 0, 0, 120, 255, 255, 135, 0, 0, 0, 0, 0, 0, 0, 0, 150, 255, 255, 95, 0, 0, 0, 0, 0, 0, 41, 255, 255, 155, 0, 0, 0, 0, 0, 0, 0, 0, 169, 255, 255, 74, 0, 0, 0, 0, 0, 0, 0, 0, 180, 255, 255, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 235, 255, 243, 1, 0;

        data.row(17) << 
        0, 59, 255, 255, 171, 0, 0, 0, 0, 0, 0, 0, 20, 254, 255, 210, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 3, 172, 255, 255, 212, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 190, 208, 191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 188, 255, 255, 42, 0, 28, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 255, 255, 85, 0, 0, 79, 255, 255, 127, 0, 0, 0, 0, 0, 0, 0, 0, 135, 255, 255, 97, 0, 0, 0, 0, 0, 0, 145, 255, 255, 55, 0, 0, 0, 0, 0, 0, 0, 0, 200, 255, 255, 30, 0, 0, 0, 0, 0, 0, 0, 0, 135, 255, 255, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 255, 255, 200, 0, 0;

        data.row(18) << 
        0, 10, 247, 255, 241, 6, 0, 0, 0, 0, 0, 0, 93, 255, 255, 152, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 0, 123, 255, 255, 187, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 201, 255, 255, 21, 0, 0, 0, 0, 0, 0, 0, 0, 221, 255, 255, 23, 0, 14, 128, 128, 128, 128, 128, 128, 128, 128, 128, 196, 255, 255, 164, 128, 128, 82, 0, 0, 144, 184, 183, 16, 0, 0, 0, 0, 0, 0, 0, 0, 188, 255, 255, 47, 0, 0, 30, 255, 255, 171, 0, 0, 0, 0, 0, 0, 0, 0, 178, 255, 255, 70, 0, 0, 0, 0, 0, 10, 240, 255, 211, 1, 0, 0, 0, 0, 0, 0, 0, 0, 187, 255, 255, 56, 0, 0, 0, 0, 0, 0, 0, 0, 162, 255, 255, 85, 0, 0, 23, 92, 92, 54, 0, 0, 0, 0, 0, 0, 0, 120, 255, 255, 124, 0, 0;

        data.row(19) << 
        0, 0, 180, 255, 255, 67, 0, 0, 0, 0, 0, 0, 169, 255, 255, 73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 7, 232, 255, 228, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 156, 255, 255, 90, 0, 0, 0, 0, 0, 0, 0, 32, 254, 255, 228, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 141, 255, 255, 109, 0, 0, 0, 0, 0, 0, 0, 35, 248, 255, 230, 2, 0, 0, 0, 205, 255, 241, 25, 0, 0, 0, 0, 0, 0, 22, 240, 255, 250, 12, 0, 0, 0, 0, 0, 81, 255, 255, 133, 0, 0, 0, 0, 0, 0, 0, 0, 0, 139, 255, 255, 125, 0, 0, 0, 0, 0, 0, 0, 7, 223, 255, 255, 37, 0, 0, 16, 251, 255, 208, 0, 0, 0, 0, 0, 0, 2, 214, 255, 255, 40, 0, 0;

        data.row(20) << 
        0, 0, 75, 255, 255, 191, 2, 0, 0, 0, 0, 45, 250, 255, 220, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 90, 255, 255, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 67, 255, 255, 226, 14, 0, 0, 0, 0, 0, 4, 185, 255, 255, 141, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 60, 255, 255, 237, 36, 0, 0, 0, 0, 0, 9, 185, 255, 255, 118, 0, 0, 0, 0, 101, 255, 255, 176, 6, 0, 0, 0, 0, 4, 170, 255, 255, 161, 0, 0, 0, 0, 0, 0, 153, 255, 255, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 255, 255, 248, 47, 0, 0, 0, 0, 0, 1, 143, 255, 255, 201, 0, 0, 0, 0, 190, 255, 255, 87, 0, 0, 0, 0, 0, 120, 255, 255, 167, 0, 0, 0;

        data.row(21) << 
        0, 0, 1, 192, 255, 255, 163, 19, 0, 1, 62, 228, 255, 255, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 147, 255, 255, 211, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 36, 0, 0, 2, 195, 255, 255, 225, 87, 28, 2, 23, 74, 207, 255, 255, 234, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 175, 255, 255, 231, 103, 26, 2, 28, 81, 216, 255, 255, 208, 10, 0, 0, 0, 0, 4, 210, 255, 255, 205, 70, 17, 14, 65, 198, 255, 255, 237, 30, 0, 0, 0, 0, 0, 1, 225, 255, 252, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 158, 255, 255, 246, 124, 41, 5, 19, 66, 190, 255, 255, 252, 59, 0, 0, 0, 0, 70, 255, 255, 246, 116, 22, 2, 36, 150, 255, 255, 237, 33, 0, 0, 0;

        data.row(22) << 
        0, 0, 0, 24, 226, 255, 255, 249, 215, 230, 255, 255, 255, 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 181, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 48, 0, 0, 0, 29, 226, 255, 255, 255, 255, 253, 255, 255, 255, 255, 236, 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 15, 199, 255, 255, 255, 255, 253, 255, 255, 255, 255, 214, 31, 0, 0, 0, 0, 0, 0, 34, 233, 255, 255, 255, 255, 255, 255, 255, 255, 239, 62, 0, 0, 0, 0, 0, 0, 41, 255, 255, 202, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 181, 255, 255, 255, 255, 253, 255, 255, 255, 255, 249, 91, 0, 0, 0, 0, 0, 0, 133, 255, 255, 255, 255, 252, 255, 255, 255, 241, 60, 0, 0, 0, 0;

        data.row(23) << 
        0, 0, 0, 0, 22, 174, 254, 255, 255, 255, 255, 236, 101, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 184, 255, 255, 28, 0, 0, 0, 0, 0, 215, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 48, 0, 0, 0, 0, 17, 158, 247, 255, 255, 255, 255, 255, 251, 162, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 255, 255, 72, 0, 0, 0, 0, 0, 0, 0, 8, 120, 236, 255, 255, 255, 255, 255, 245, 138, 14, 0, 0, 0, 0, 0, 0, 0, 0, 26, 172, 253, 255, 255, 255, 255, 255, 173, 35, 0, 0, 0, 0, 0, 0, 0, 113, 255, 255, 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 108, 227, 255, 255, 255, 255, 255, 255, 183, 49, 0, 0, 0, 0, 0, 0, 0, 1, 93, 232, 255, 255, 255, 255, 253, 176, 30, 0, 0, 0, 0, 0;

        data.row(24) << 
        0, 0, 0, 0, 0, 0, 30, 94, 120, 110, 74, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 77, 104, 123, 110, 76, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 60, 101, 122, 109, 73, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 89, 116, 117, 89, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 52, 102, 122, 115, 87, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 59, 102, 117, 88, 25, 0, 0, 0, 0, 0, 0, 0;

        data.row(25) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        data.row(26) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        data.row(27) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        data.row(28) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        data.row(29) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        data.row(30) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        data.row(31) << 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

        return data;
}

