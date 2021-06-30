/*
  Copyright (c) 2010-2011, Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.


   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
uniform const float ONE_OVER_FOUR = 1.0f/4.0f;
uniform const float PI_OVER_SIXTEEN = PI/16.0f;

inline float normalizeing_scale_factor(int32 a)
{
    return (a==0)?1.0f/sqrt(2.0f) : 1.0f;
}

export void DCT8X8(uniform const int32 in_matrix[64],uniform float out_matrix[64]) {
    foreach(i = 0...64)
    {
        out_matrix[i] = 0;
    }
    uniform float chache[8][8][8][8];
    foreach (u = 0...8,v = 0...8,x = 0...8,y = 0...8) {
         int32 val = in_matrix[x*8+y];
         float cu = cos((2.0f*x+1.0f)*u*PI_OVER_SIXTEEN);
         float cv = cos((2.0f*y+1.0f)*u*PI_OVER_SIXTEEN);

         chache[u][v][x][y] =  normalizeing_scale_factor(u) * normalizeing_scale_factor(v) * ONE_OVER_FOUR * cu * cv * val;
         
    }
   
    foreach (u = 0...8,v = 0...8) {
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                out_matrix[u*8+v] += chache[u][v][x][y];
            }
        }

    }
}

export void IDCT8X8(uniform const int32 G[64], uniform int32 g[64])
{
    uniform float result_cache[8][8][8][8];

    foreach(x = 0 ... 8, y = 0 ... 8, u = 0 ... 8, v = 0 ... 8) {
        float scale_factor = ONE_OVER_FOUR * normalizeing_scale_factor(u)
            * normalizeing_scale_factor(v);
        float normial = G[u*8+v] * cos((2.0f * x + 1.0f) * u * PI_OVER_SIXTEEN)
            * cos((2.0f * y + 1.0f) * v * PI_OVER_SIXTEEN);
        result_cache[x][y][u][v] = scale_factor * normial;
    }

    foreach(x = 0 ... 8, y = 0 ... 8) {
        float sum = 0.0f;
        for (int u = 0; u < 8; u++) {
            for (int v = 0; v < 8; v++) {
                sum += result_cache[x][y][u][v];
            }
        }
        g[x*8+y] = round(sum);
    }
}