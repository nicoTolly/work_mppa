__kernel void kernel0(__global float *Lattices, int c0)
{
    int b0 = get_group_id(0), b1 = get_group_id(1);
    int t0_0 = get_local_id(0), t1_0 = get_local_id(1), t2_0 = get_local_id(2);
    float private_rho;
    float private_e;
    float private_eps;
    float private_jx;
    float private_qx;
    float private_jy;
    float private_qy;
    float private_jz;
    float private_qz;
    float private_px;
    float private_pix;
    float private_pw;
    float private_piw;
    float private_pxy;
    float private_pyz;
    float private_pxz;
    float private_mx;
    float private_my;
    float private_mz;
    float private_t0;
    float private_t1;
    float private_t2;
    float private_t3;
    float private_f0;
    float private_f1;
    float private_f2;
    float private_f3;
    float private_f4;
    float private_f5;
    float private_f6;
    float private_f7;
    float private_f8;
    float private_f9;
    float private_f10;
    float private_f11;
    float private_f12;
    float private_f13;
    float private_f14;
    float private_f15;
    float private_f16;
    float private_f17;
    float private_f18;
    int private_g;

    #define ppcg_min(x,y)    min((__typeof__(x + y)) x, (__typeof__(x + y)) y)
    #define ppcg_max(x,y)    max((__typeof__(x + y)) x, (__typeof__(x + y)) y)
    if ((63 * b0 - b1 + 255) % 256 >= 188) {
      if (b0 >= 1 && b0 <= 6) {
        for (int c3 = 0; c3 <= 127; c3 += 16)
          for (int c5 = ppcg_max(t1_0, 16 * ((63 * b0 - b1 + 255) % 256) + ((t0_0 + t1_0 + 4) % 4) + 63 * t0_0 - 4080); c5 <= ppcg_min(15, 16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - 3953); c5 += 4)
            for (int c6 = t2_0; c6 <= 15; c6 += 4) {
              private_g = 0;
              if (t2_0 == 3 && c3 == 112 && c6 == 15) {
                private_g = (private_g | 5441);
              } else if (t2_0 == 0 && c3 == 0 && c6 == 0)
                private_g = (private_g | 10882);
              if ((3 * t0_0 - t1_0) % 4 == 0 && (1008 * b0 + 4080 * b1 + 63 * t0_0 - c5) % 4096 == 0) {
                private_g = (private_g | 164616);
              } else if (t0_0 + c5 == 15 && (-b0 + 3 * b1 + 3) % 4 == 0 && (3 * b0 + 3 * b1 - 4 * t1_0 - 9) % 16 == 0 && (63 * b0 - b1 + 4 * t0_0 - 249) % 256 == 0)
                private_g = (private_g | 82116);
              private_f0 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 0];
              if (private_g & 262143) {
                if (private_g & 2) {
                  private_f1 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 2];
                } else {
                  private_f1 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                }
                if (private_g & 1) {
                  private_f2 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 1];
                } else {
                  private_f2 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                }
                if (private_g & 8) {
                  private_f3 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 4];
                } else {
                  private_f3 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6)) * 19 + 3];
                }
                if (private_g & 4) {
                  private_f4 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 3];
                } else {
                  private_f4 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6)) * 19 + 4];
                }
                if (private_g & 32) {
                  private_f5 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 6];
                } else {
                  private_f5 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 5];
                }
                if (private_g & 16) {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 5];
                } else {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 6];
                }
                if (private_g & 512) {
                  private_f7 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 10];
                } else {
                  private_f7 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                }
                if (private_g & 256) {
                  private_f8 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 9];
                } else {
                  private_f8 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                }
                if (private_g & 128) {
                  private_f9 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 8];
                } else {
                  private_f9 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                }
                if (private_g & 64) {
                  private_f10 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 7];
                } else {
                  private_f10 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                }
                if (private_g & 8192) {
                  private_f11 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 14];
                } else {
                  private_f11 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                }
                if (private_g & 4096) {
                  private_f12 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 13];
                } else {
                  private_f12 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                }
                if (private_g & 2048) {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 12];
                } else {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                }
                if (private_g & 1024) {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 11];
                } else {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                }
                if (private_g & 131072) {
                  private_f15 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 18];
                } else {
                  private_f15 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6)) * 19 + 15];
                }
                if (private_g & 65536) {
                  private_f16 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 17];
                } else {
                  private_f16 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6)) * 19 + 16];
                }
                if (private_g & 32768) {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 16];
                } else {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6)) * 19 + 17];
                }
                if (private_g & 16384) {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 15];
                } else {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6)) * 19 + 18];
                }
                if ((0)) {
                  private_f13 += (0.400000006F / 6);
                  private_f14 -= (0.400000006F / 6);
                }
              } else {
                private_f1 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                private_f2 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                private_f3 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6)) * 19 + 3];
                private_f4 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6)) * 19 + 4];
                private_f5 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 5];
                private_f6 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 6];
                private_f7 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                private_f8 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                private_f9 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                private_f10 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                private_f11 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                private_f12 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                private_f13 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                private_f14 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                private_f15 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6)) * 19 + 15];
                private_f16 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 - 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6)) * 19 + 16];
                private_f17 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256) + ((16 * ((63 * b0 - b1 + 255) % 256) + 63 * t0_0 - c5 + 4294963216) % 4294967296)) + 1008 * b0 - 16 * b1 - 4096 * ((63 * b0 - b1 + 255) / 256) + 4294971375)) * 128 + (c3 + c6)) * 19 + 17];
                private_f18 = Lattices[((((c0 % 2) * 128 + (16 * b0 + t0_0 + 1)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4081)) * 128 + (c3 + c6)) * 19 + 18];
              }
              private_t0 = (((((private_f1 + private_f2) + private_f3) + private_f4) + private_f5) + private_f6);
              private_t1 = (((((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) + private_f15) + private_f16) + private_f17) + private_f18);
              private_rho = ((private_f0 + private_t0) + private_t1);
              private_e = ((((-30) * private_f0) - (11 * private_t0)) + (8 * private_t1));
              private_eps = (((12 * private_f0) - (4 * private_t0)) + private_t1);
              private_t0 = (private_f1 - private_f2);
              private_t1 = (((((((private_f7 - private_f8) + private_f9) - private_f10) + private_f11) - private_f12) + private_f13) - private_f14);
              private_jx = (private_t0 + private_t1);
              private_qx = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f3 - private_f4);
              private_t1 = (((((((private_f7 + private_f8) - private_f9) - private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_jy = (private_t0 + private_t1);
              private_qy = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f5 - private_f6);
              private_t1 = (((((((private_f11 + private_f12) - private_f13) - private_f14) + private_f15) + private_f16) - private_f17) - private_f18);
              private_jz = (private_t0 + private_t1);
              private_qz = (((-4) * private_t0) + private_t1);
              private_t0 = ((2 * (private_f1 + private_f2)) - (((private_f3 + private_f4) + private_f5) + private_f6));
              private_t1 = ((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) - (2 * (((private_f15 + private_f16) + private_f17) + private_f18)));
              private_px = (private_t0 + private_t1);
              private_pix = (((-2) * private_t0) + private_t1);
              private_t0 = ((private_f3 + private_f4) - (private_f5 + private_f6));
              private_t1 = ((((private_f7 + private_f8) + private_f9) + private_f10) - (((private_f11 + private_f12) + private_f13) + private_f14));
              private_pw = (private_t0 + private_t1);
              private_piw = (((-2) * private_t0) + private_t1);
              private_pxy = (((private_f7 - private_f8) - private_f9) + private_f10);
              private_pyz = (((private_f15 - private_f16) - private_f17) + private_f18);
              private_pxz = (((private_f11 - private_f12) - private_f13) + private_f14);
              private_mx = (((((((private_f7 - private_f8) + private_f9) - private_f10) - private_f11) + private_f12) - private_f13) + private_f14);
              private_my = ((((((((-private_f7) - private_f8) + private_f9) + private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_mz = (((((((private_f11 + private_f12) - private_f13) - private_f14) - private_f15) - private_f16) + private_f17) + private_f18);
              private_t0 = (private_jx * private_jx);
              private_t1 = (private_jy * private_jy);
              private_t2 = (private_jz * private_jz);
              private_e += (1.19000006F * ((((-11) * private_rho) + (19 * ((private_t0 + private_t1) + private_t2))) - private_e));
              private_eps += (1.39999998F * ((3 * private_rho) - private_eps));
              private_qx += (1.20000005F * ((((-2.F) / 3) * private_jx) - private_qx));
              private_qy += (1.20000005F * ((((-2.F) / 3) * private_jy) - private_qy));
              private_qz += (1.20000005F * ((((-2.F) / 3) * private_jz) - private_qz));
              private_px += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((((2 * private_t0) - private_t1) - private_t2) - private_px));
              private_pix += ((-1.39999998F) * private_pix);
              private_pw += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_t1 - private_t2) - private_pw));
              private_piw += ((-1.39999998F) * private_piw);
              private_pxy += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jx * private_jy) - private_pxy));
              private_pyz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jy * private_jz) - private_pyz));
              private_pxz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jz * private_jx) - private_pxz));
              private_mx += ((-1.98000002F) * private_mx);
              private_my += ((-1.98000002F) * private_my);
              private_mz += ((-1.98000002F) * private_mz);
              private_f0 = ((((1.F / 19) * private_rho) - ((5.F / 399) * private_e)) + ((1.F / 21) * private_eps));
              private_t0 = ((((1.F / 19) * private_rho) - ((11.F / 2394) * private_e)) - ((1.F / 63) * private_eps));
              private_t1 = (private_px - private_pix);
              private_t2 = (private_pw - private_piw);
              private_f1 = ((private_t0 + ((1.F / 18) * private_t1)) + ((1.F / 10) * (private_jx - private_qx)));
              private_f2 = ((private_t0 + ((1.F / 18) * private_t1)) - ((1.F / 10) * (private_jx - private_qx)));
              private_f3 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jy - private_qy)));
              private_f4 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jy - private_qy)));
              private_f5 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jz - private_qz)));
              private_f6 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jz - private_qz)));
              private_t0 = ((((1.F / 19) * private_rho) + ((4.F / 1197) * private_e)) + ((1.F / 252) * private_eps));
              private_t1 = (((1.F / 10) * private_jx) + ((1.F / 40) * private_qx));
              private_t2 = (((1.F / 10) * private_jy) + ((1.F / 40) * private_qy));
              private_t3 = (((1.F / 10) * private_jz) + ((1.F / 40) * private_qz));
              private_f7 = ((((((((private_t0 + private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx - private_my)));
              private_f8 = ((((((((private_t0 - private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx + private_my)));
              private_f9 = ((((((((private_t0 + private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx + private_my)));
              private_f10 = ((((((((private_t0 - private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx - private_my)));
              private_f11 = ((((((((private_t0 + private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx - private_mz)));
              private_f12 = ((((((((private_t0 - private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx + private_mz)));
              private_f13 = ((((((((private_t0 + private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx + private_mz)));
              private_f14 = ((((((((private_t0 - private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx - private_mz)));
              private_f15 = ((((((private_t0 + private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my - private_mz)));
              private_f16 = ((((((private_t0 - private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my + private_mz)));
              private_f17 = ((((((private_t0 + private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my + private_mz)));
              private_f18 = ((((((private_t0 - private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my - private_mz)));
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 0] = private_f0;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 1] = private_f1;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 2] = private_f2;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 3] = private_f3;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 4] = private_f4;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 5] = private_f5;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 6] = private_f6;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 7] = private_f7;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 8] = private_f8;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 9] = private_f9;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 10] = private_f10;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 11] = private_f11;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 12] = private_f12;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 13] = private_f13;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 14] = private_f14;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 15] = private_f15;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 16] = private_f16;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 17] = private_f17;
              Lattices[(((((c0 + 1) % 2) * 128 + (16 * b0 + t0_0)) * 128 + (-(16 * ((63 * b0 - b1 + 255) % 256)) - 63 * t0_0 + c5 + 4080)) * 128 + (c3 + c6)) * 19 + 18] = private_f18;
            }
      } else if (b0 == 7 && b1 >= 244) {
        for (int c3 = 0; c3 <= 127; c3 += 16)
          for (int c5 = ppcg_max(t1_0, ((t0_0 + t1_0 + 4) % 4) - 16 * b1 + 63 * t0_0 + 2960); c5 <= ppcg_min(15, -16 * b1 + 63 * t0_0 + 3087); c5 += 4)
            for (int c6 = t2_0; c6 <= 15; c6 += 4) {
              private_g = 0;
              if (t2_0 == 3 && c3 == 112 && c6 == 15) {
                private_g = (private_g | 5441);
              } else if (t2_0 == 0 && c3 == 0 && c6 == 0)
                private_g = (private_g | 10882);
              if (b1 == 244 && t0_0 == 15 && t1_0 == 1 && c5 == 1) {
                private_g = (private_g | 164616);
              } else if (4 * t0_0 + 192 == b1 && b1 + 4 * t1_0 == 252 && b1 + 4 * c5 == 252)
                private_g = (private_g | 82116);
              if (t0_0 == 15)
                private_g = (private_g | 52240);
              private_f0 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 0];
              if (private_g & 262143) {
                if (private_g & 2) {
                  private_f1 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 2];
                } else {
                  private_f1 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                }
                if (private_g & 1) {
                  private_f2 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 1];
                } else {
                  private_f2 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                }
                if (private_g & 8) {
                  private_f3 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 4];
                } else {
                  private_f3 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                }
                if (private_g & 4) {
                  private_f4 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 3];
                } else {
                  private_f4 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 4];
                }
                if (private_g & 32) {
                  private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6];
                } else {
                  private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5];
                }
                if (private_g & 16) {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5];
                } else {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6];
                }
                if (private_g & 512) {
                  private_f7 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 10];
                } else {
                  private_f7 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                }
                if (private_g & 256) {
                  private_f8 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 9];
                } else {
                  private_f8 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                }
                if (private_g & 128) {
                  private_f9 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 8];
                } else {
                  private_f9 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                }
                if (private_g & 64) {
                  private_f10 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 7];
                } else {
                  private_f10 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                }
                if (private_g & 8192) {
                  private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 14];
                } else {
                  private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                }
                if (private_g & 4096) {
                  private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 13];
                } else {
                  private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                }
                if (private_g & 2048) {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 12];
                } else {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                }
                if (private_g & 1024) {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 11];
                } else {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                }
                if (private_g & 131072) {
                  private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 18];
                } else {
                  private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                }
                if (private_g & 65536) {
                  private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 17];
                } else {
                  private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 16];
                }
                if (private_g & 32768) {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 16];
                } else {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                }
                if (private_g & 16384) {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 15];
                } else {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 18];
                }
                if ((t0_0 <= 14 ? 0 : 1)) {
                  private_f13 += (0.400000006F / 6);
                  private_f14 -= (0.400000006F / 6);
                }
              } else {
                private_f1 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                private_f2 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                private_f3 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                private_f4 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 4];
                private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5];
                private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6];
                private_f7 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                private_f8 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                private_f9 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                private_f10 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 16];
                private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 18];
              }
              private_t0 = (((((private_f1 + private_f2) + private_f3) + private_f4) + private_f5) + private_f6);
              private_t1 = (((((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) + private_f15) + private_f16) + private_f17) + private_f18);
              private_rho = ((private_f0 + private_t0) + private_t1);
              private_e = ((((-30) * private_f0) - (11 * private_t0)) + (8 * private_t1));
              private_eps = (((12 * private_f0) - (4 * private_t0)) + private_t1);
              private_t0 = (private_f1 - private_f2);
              private_t1 = (((((((private_f7 - private_f8) + private_f9) - private_f10) + private_f11) - private_f12) + private_f13) - private_f14);
              private_jx = (private_t0 + private_t1);
              private_qx = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f3 - private_f4);
              private_t1 = (((((((private_f7 + private_f8) - private_f9) - private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_jy = (private_t0 + private_t1);
              private_qy = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f5 - private_f6);
              private_t1 = (((((((private_f11 + private_f12) - private_f13) - private_f14) + private_f15) + private_f16) - private_f17) - private_f18);
              private_jz = (private_t0 + private_t1);
              private_qz = (((-4) * private_t0) + private_t1);
              private_t0 = ((2 * (private_f1 + private_f2)) - (((private_f3 + private_f4) + private_f5) + private_f6));
              private_t1 = ((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) - (2 * (((private_f15 + private_f16) + private_f17) + private_f18)));
              private_px = (private_t0 + private_t1);
              private_pix = (((-2) * private_t0) + private_t1);
              private_t0 = ((private_f3 + private_f4) - (private_f5 + private_f6));
              private_t1 = ((((private_f7 + private_f8) + private_f9) + private_f10) - (((private_f11 + private_f12) + private_f13) + private_f14));
              private_pw = (private_t0 + private_t1);
              private_piw = (((-2) * private_t0) + private_t1);
              private_pxy = (((private_f7 - private_f8) - private_f9) + private_f10);
              private_pyz = (((private_f15 - private_f16) - private_f17) + private_f18);
              private_pxz = (((private_f11 - private_f12) - private_f13) + private_f14);
              private_mx = (((((((private_f7 - private_f8) + private_f9) - private_f10) - private_f11) + private_f12) - private_f13) + private_f14);
              private_my = ((((((((-private_f7) - private_f8) + private_f9) + private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_mz = (((((((private_f11 + private_f12) - private_f13) - private_f14) - private_f15) - private_f16) + private_f17) + private_f18);
              private_t0 = (private_jx * private_jx);
              private_t1 = (private_jy * private_jy);
              private_t2 = (private_jz * private_jz);
              private_e += (1.19000006F * ((((-11) * private_rho) + (19 * ((private_t0 + private_t1) + private_t2))) - private_e));
              private_eps += (1.39999998F * ((3 * private_rho) - private_eps));
              private_qx += (1.20000005F * ((((-2.F) / 3) * private_jx) - private_qx));
              private_qy += (1.20000005F * ((((-2.F) / 3) * private_jy) - private_qy));
              private_qz += (1.20000005F * ((((-2.F) / 3) * private_jz) - private_qz));
              private_px += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((((2 * private_t0) - private_t1) - private_t2) - private_px));
              private_pix += ((-1.39999998F) * private_pix);
              private_pw += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_t1 - private_t2) - private_pw));
              private_piw += ((-1.39999998F) * private_piw);
              private_pxy += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jx * private_jy) - private_pxy));
              private_pyz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jy * private_jz) - private_pyz));
              private_pxz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jz * private_jx) - private_pxz));
              private_mx += ((-1.98000002F) * private_mx);
              private_my += ((-1.98000002F) * private_my);
              private_mz += ((-1.98000002F) * private_mz);
              private_f0 = ((((1.F / 19) * private_rho) - ((5.F / 399) * private_e)) + ((1.F / 21) * private_eps));
              private_t0 = ((((1.F / 19) * private_rho) - ((11.F / 2394) * private_e)) - ((1.F / 63) * private_eps));
              private_t1 = (private_px - private_pix);
              private_t2 = (private_pw - private_piw);
              private_f1 = ((private_t0 + ((1.F / 18) * private_t1)) + ((1.F / 10) * (private_jx - private_qx)));
              private_f2 = ((private_t0 + ((1.F / 18) * private_t1)) - ((1.F / 10) * (private_jx - private_qx)));
              private_f3 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jy - private_qy)));
              private_f4 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jy - private_qy)));
              private_f5 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jz - private_qz)));
              private_f6 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jz - private_qz)));
              private_t0 = ((((1.F / 19) * private_rho) + ((4.F / 1197) * private_e)) + ((1.F / 252) * private_eps));
              private_t1 = (((1.F / 10) * private_jx) + ((1.F / 40) * private_qx));
              private_t2 = (((1.F / 10) * private_jy) + ((1.F / 40) * private_qy));
              private_t3 = (((1.F / 10) * private_jz) + ((1.F / 40) * private_qz));
              private_f7 = ((((((((private_t0 + private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx - private_my)));
              private_f8 = ((((((((private_t0 - private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx + private_my)));
              private_f9 = ((((((((private_t0 + private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx + private_my)));
              private_f10 = ((((((((private_t0 - private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx - private_my)));
              private_f11 = ((((((((private_t0 + private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx - private_mz)));
              private_f12 = ((((((((private_t0 - private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx + private_mz)));
              private_f13 = ((((((((private_t0 + private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx + private_mz)));
              private_f14 = ((((((((private_t0 - private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx - private_mz)));
              private_f15 = ((((((private_t0 + private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my - private_mz)));
              private_f16 = ((((((private_t0 - private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my + private_mz)));
              private_f17 = ((((((private_t0 + private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my + private_mz)));
              private_f18 = ((((((private_t0 - private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my - private_mz)));
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 0] = private_f0;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 1] = private_f1;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 2] = private_f2;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 3] = private_f3;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 4] = private_f4;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5] = private_f5;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6] = private_f6;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 7] = private_f7;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 8] = private_f8;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 9] = private_f9;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 10] = private_f10;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 11] = private_f11;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 12] = private_f12;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 13] = private_f13;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 14] = private_f14;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 15] = private_f15;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 16] = private_f16;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 17] = private_f17;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 18] = private_f18;
            }
      } else if (b0 == 7 && b1 <= 243) {
        for (int c3 = 0; c3 <= 127; c3 += 16)
          for (int c5 = ppcg_max(t1_0, ((t0_0 + t1_0 + 4) % 4) - 16 * b1 + 63 * t0_0 + 2960); c5 <= ppcg_min(15, -16 * b1 + 63 * t0_0 + 3087); c5 += 4)
            for (int c6 = t2_0; c6 <= 15; c6 += 4) {
              private_g = 0;
              if (t2_0 == 3 && c3 == 112 && c6 == 15) {
                private_g = (private_g | 5441);
              } else if (t2_0 == 0 && c3 == 0 && c6 == 0)
                private_g = (private_g | 10882);
              if (16 * b1 + c5 == 63 * t0_0 + 2960) {
                private_g = (private_g | 164616);
              } else if (4 * t0_0 + 192 == b1 && b1 + 4 * c5 == 252)
                private_g = (private_g | 82116);
              private_f0 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 0];
              if (private_g & 262143) {
                if (private_g & 2) {
                  private_f1 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 2];
                } else {
                  private_f1 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                }
                if (private_g & 1) {
                  private_f2 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 1];
                } else {
                  private_f2 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                }
                if (private_g & 8) {
                  private_f3 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 4];
                } else {
                  private_f3 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                }
                if (private_g & 4) {
                  private_f4 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 3];
                } else {
                  private_f4 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 4];
                }
                if (private_g & 32) {
                  private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6];
                } else {
                  private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5];
                }
                if (private_g & 16) {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5];
                } else {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6];
                }
                if (private_g & 512) {
                  private_f7 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 10];
                } else {
                  private_f7 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                }
                if (private_g & 256) {
                  private_f8 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 9];
                } else {
                  private_f8 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                }
                if (private_g & 128) {
                  private_f9 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 8];
                } else {
                  private_f9 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                }
                if (private_g & 64) {
                  private_f10 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 7];
                } else {
                  private_f10 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                }
                if (private_g & 8192) {
                  private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 14];
                } else {
                  private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                }
                if (private_g & 4096) {
                  private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 13];
                } else {
                  private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                }
                if (private_g & 2048) {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 12];
                } else {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                }
                if (private_g & 1024) {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 11];
                } else {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                }
                if (private_g & 131072) {
                  private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 18];
                } else {
                  private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                }
                if (private_g & 65536) {
                  private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 17];
                } else {
                  private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 16];
                }
                if (private_g & 32768) {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 16];
                } else {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                }
                if (private_g & 16384) {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 15];
                } else {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 18];
                }
                if ((0)) {
                  private_f13 += (0.400000006F / 6);
                  private_f14 -= (0.400000006F / 6);
                }
              } else {
                private_f1 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                private_f2 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                private_f3 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                private_f4 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 4];
                private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5];
                private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6];
                private_f7 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                private_f8 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                private_f9 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                private_f10 = Lattices[((((c0 % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 + 111)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 16];
                private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294964335) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 113)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2959)) * 128 + (c3 + c6)) * 19 + 18];
              }
              private_t0 = (((((private_f1 + private_f2) + private_f3) + private_f4) + private_f5) + private_f6);
              private_t1 = (((((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) + private_f15) + private_f16) + private_f17) + private_f18);
              private_rho = ((private_f0 + private_t0) + private_t1);
              private_e = ((((-30) * private_f0) - (11 * private_t0)) + (8 * private_t1));
              private_eps = (((12 * private_f0) - (4 * private_t0)) + private_t1);
              private_t0 = (private_f1 - private_f2);
              private_t1 = (((((((private_f7 - private_f8) + private_f9) - private_f10) + private_f11) - private_f12) + private_f13) - private_f14);
              private_jx = (private_t0 + private_t1);
              private_qx = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f3 - private_f4);
              private_t1 = (((((((private_f7 + private_f8) - private_f9) - private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_jy = (private_t0 + private_t1);
              private_qy = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f5 - private_f6);
              private_t1 = (((((((private_f11 + private_f12) - private_f13) - private_f14) + private_f15) + private_f16) - private_f17) - private_f18);
              private_jz = (private_t0 + private_t1);
              private_qz = (((-4) * private_t0) + private_t1);
              private_t0 = ((2 * (private_f1 + private_f2)) - (((private_f3 + private_f4) + private_f5) + private_f6));
              private_t1 = ((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) - (2 * (((private_f15 + private_f16) + private_f17) + private_f18)));
              private_px = (private_t0 + private_t1);
              private_pix = (((-2) * private_t0) + private_t1);
              private_t0 = ((private_f3 + private_f4) - (private_f5 + private_f6));
              private_t1 = ((((private_f7 + private_f8) + private_f9) + private_f10) - (((private_f11 + private_f12) + private_f13) + private_f14));
              private_pw = (private_t0 + private_t1);
              private_piw = (((-2) * private_t0) + private_t1);
              private_pxy = (((private_f7 - private_f8) - private_f9) + private_f10);
              private_pyz = (((private_f15 - private_f16) - private_f17) + private_f18);
              private_pxz = (((private_f11 - private_f12) - private_f13) + private_f14);
              private_mx = (((((((private_f7 - private_f8) + private_f9) - private_f10) - private_f11) + private_f12) - private_f13) + private_f14);
              private_my = ((((((((-private_f7) - private_f8) + private_f9) + private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_mz = (((((((private_f11 + private_f12) - private_f13) - private_f14) - private_f15) - private_f16) + private_f17) + private_f18);
              private_t0 = (private_jx * private_jx);
              private_t1 = (private_jy * private_jy);
              private_t2 = (private_jz * private_jz);
              private_e += (1.19000006F * ((((-11) * private_rho) + (19 * ((private_t0 + private_t1) + private_t2))) - private_e));
              private_eps += (1.39999998F * ((3 * private_rho) - private_eps));
              private_qx += (1.20000005F * ((((-2.F) / 3) * private_jx) - private_qx));
              private_qy += (1.20000005F * ((((-2.F) / 3) * private_jy) - private_qy));
              private_qz += (1.20000005F * ((((-2.F) / 3) * private_jz) - private_qz));
              private_px += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((((2 * private_t0) - private_t1) - private_t2) - private_px));
              private_pix += ((-1.39999998F) * private_pix);
              private_pw += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_t1 - private_t2) - private_pw));
              private_piw += ((-1.39999998F) * private_piw);
              private_pxy += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jx * private_jy) - private_pxy));
              private_pyz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jy * private_jz) - private_pyz));
              private_pxz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jz * private_jx) - private_pxz));
              private_mx += ((-1.98000002F) * private_mx);
              private_my += ((-1.98000002F) * private_my);
              private_mz += ((-1.98000002F) * private_mz);
              private_f0 = ((((1.F / 19) * private_rho) - ((5.F / 399) * private_e)) + ((1.F / 21) * private_eps));
              private_t0 = ((((1.F / 19) * private_rho) - ((11.F / 2394) * private_e)) - ((1.F / 63) * private_eps));
              private_t1 = (private_px - private_pix);
              private_t2 = (private_pw - private_piw);
              private_f1 = ((private_t0 + ((1.F / 18) * private_t1)) + ((1.F / 10) * (private_jx - private_qx)));
              private_f2 = ((private_t0 + ((1.F / 18) * private_t1)) - ((1.F / 10) * (private_jx - private_qx)));
              private_f3 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jy - private_qy)));
              private_f4 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jy - private_qy)));
              private_f5 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jz - private_qz)));
              private_f6 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jz - private_qz)));
              private_t0 = ((((1.F / 19) * private_rho) + ((4.F / 1197) * private_e)) + ((1.F / 252) * private_eps));
              private_t1 = (((1.F / 10) * private_jx) + ((1.F / 40) * private_qx));
              private_t2 = (((1.F / 10) * private_jy) + ((1.F / 40) * private_qy));
              private_t3 = (((1.F / 10) * private_jz) + ((1.F / 40) * private_qz));
              private_f7 = ((((((((private_t0 + private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx - private_my)));
              private_f8 = ((((((((private_t0 - private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx + private_my)));
              private_f9 = ((((((((private_t0 + private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx + private_my)));
              private_f10 = ((((((((private_t0 - private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx - private_my)));
              private_f11 = ((((((((private_t0 + private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx - private_mz)));
              private_f12 = ((((((((private_t0 - private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx + private_mz)));
              private_f13 = ((((((((private_t0 + private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx + private_mz)));
              private_f14 = ((((((((private_t0 - private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx - private_mz)));
              private_f15 = ((((((private_t0 + private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my - private_mz)));
              private_f16 = ((((((private_t0 - private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my + private_mz)));
              private_f17 = ((((((private_t0 + private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my + private_mz)));
              private_f18 = ((((((private_t0 - private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my - private_mz)));
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 0] = private_f0;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 1] = private_f1;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 2] = private_f2;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 3] = private_f3;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 4] = private_f4;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 5] = private_f5;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 6] = private_f6;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 7] = private_f7;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 8] = private_f8;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 9] = private_f9;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 10] = private_f10;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 11] = private_f11;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 12] = private_f12;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 13] = private_f13;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 14] = private_f14;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 15] = private_f15;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 16] = private_f16;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 17] = private_f17;
              Lattices[(((((c0 + 1) % 2) * 128 + (t0_0 + 112)) * 128 + (16 * b1 - 63 * t0_0 + c5 - 2960)) * 128 + (c3 + c6)) * 19 + 18] = private_f18;
            }
      } else if (b1 >= 8) {
        for (int c3 = 0; c3 <= 127; c3 += 16)
          for (int c5 = ppcg_max(t1_0, ((t0_0 + t1_0 + 4) % 4) - 16 * b1 + 63 * t0_0); c5 <= ppcg_min(15, -16 * b1 + 63 * t0_0 + 127); c5 += 4)
            for (int c6 = t2_0; c6 <= 15; c6 += 4) {
              private_g = 0;
              if (t2_0 == 3 && c3 == 112 && c6 == 15) {
                private_g = (private_g | 5441);
              } else if (t2_0 == 0 && c3 == 0 && c6 == 0)
                private_g = (private_g | 10882);
              if (4 * t0_0 == b1 + 1 && b1 + 4 * c5 == 63) {
                private_g = (private_g | 164616);
              } else if (4 * t0_0 + 7 == b1 && b1 + 4 * c5 == 67)
                private_g = (private_g | 82116);
              private_f0 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 0];
              if (private_g & 262143) {
                if (private_g & 2) {
                  private_f1 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 2];
                } else {
                  private_f1 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                }
                if (private_g & 1) {
                  private_f2 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 1];
                } else {
                  private_f2 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                }
                if (private_g & 8) {
                  private_f3 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 4];
                } else {
                  private_f3 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                }
                if (private_g & 4) {
                  private_f4 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 3];
                } else {
                  private_f4 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 4];
                }
                if (private_g & 32) {
                  private_f5 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6];
                } else {
                  private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5];
                }
                if (private_g & 16) {
                  private_f6 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5];
                } else {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6];
                }
                if (private_g & 512) {
                  private_f7 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 10];
                } else {
                  private_f7 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                }
                if (private_g & 256) {
                  private_f8 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 9];
                } else {
                  private_f8 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                }
                if (private_g & 128) {
                  private_f9 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 8];
                } else {
                  private_f9 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                }
                if (private_g & 64) {
                  private_f10 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 7];
                } else {
                  private_f10 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                }
                if (private_g & 8192) {
                  private_f11 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 14];
                } else {
                  private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                }
                if (private_g & 4096) {
                  private_f12 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 13];
                } else {
                  private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                }
                if (private_g & 2048) {
                  private_f13 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 12];
                } else {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                }
                if (private_g & 1024) {
                  private_f14 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 11];
                } else {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                }
                if (private_g & 131072) {
                  private_f15 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 18];
                } else {
                  private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                }
                if (private_g & 65536) {
                  private_f16 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 17];
                } else {
                  private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 16];
                }
                if (private_g & 32768) {
                  private_f17 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 16];
                } else {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                }
                if (private_g & 16384) {
                  private_f18 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 15];
                } else {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 18];
                }
                if ((0)) {
                  private_f13 += (0.400000006F / 6);
                  private_f14 -= (0.400000006F / 6);
                }
              } else {
                private_f1 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                private_f2 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                private_f3 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                private_f4 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 4];
                private_f5 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5];
                private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6];
                private_f7 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                private_f8 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                private_f9 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                private_f10 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                private_f11 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                private_f12 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                private_f15 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                private_f16 = Lattices[((((c0 % 2) * 128 + (t0_0 - 1)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 16];
                private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 18];
              }
              private_t0 = (((((private_f1 + private_f2) + private_f3) + private_f4) + private_f5) + private_f6);
              private_t1 = (((((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) + private_f15) + private_f16) + private_f17) + private_f18);
              private_rho = ((private_f0 + private_t0) + private_t1);
              private_e = ((((-30) * private_f0) - (11 * private_t0)) + (8 * private_t1));
              private_eps = (((12 * private_f0) - (4 * private_t0)) + private_t1);
              private_t0 = (private_f1 - private_f2);
              private_t1 = (((((((private_f7 - private_f8) + private_f9) - private_f10) + private_f11) - private_f12) + private_f13) - private_f14);
              private_jx = (private_t0 + private_t1);
              private_qx = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f3 - private_f4);
              private_t1 = (((((((private_f7 + private_f8) - private_f9) - private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_jy = (private_t0 + private_t1);
              private_qy = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f5 - private_f6);
              private_t1 = (((((((private_f11 + private_f12) - private_f13) - private_f14) + private_f15) + private_f16) - private_f17) - private_f18);
              private_jz = (private_t0 + private_t1);
              private_qz = (((-4) * private_t0) + private_t1);
              private_t0 = ((2 * (private_f1 + private_f2)) - (((private_f3 + private_f4) + private_f5) + private_f6));
              private_t1 = ((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) - (2 * (((private_f15 + private_f16) + private_f17) + private_f18)));
              private_px = (private_t0 + private_t1);
              private_pix = (((-2) * private_t0) + private_t1);
              private_t0 = ((private_f3 + private_f4) - (private_f5 + private_f6));
              private_t1 = ((((private_f7 + private_f8) + private_f9) + private_f10) - (((private_f11 + private_f12) + private_f13) + private_f14));
              private_pw = (private_t0 + private_t1);
              private_piw = (((-2) * private_t0) + private_t1);
              private_pxy = (((private_f7 - private_f8) - private_f9) + private_f10);
              private_pyz = (((private_f15 - private_f16) - private_f17) + private_f18);
              private_pxz = (((private_f11 - private_f12) - private_f13) + private_f14);
              private_mx = (((((((private_f7 - private_f8) + private_f9) - private_f10) - private_f11) + private_f12) - private_f13) + private_f14);
              private_my = ((((((((-private_f7) - private_f8) + private_f9) + private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_mz = (((((((private_f11 + private_f12) - private_f13) - private_f14) - private_f15) - private_f16) + private_f17) + private_f18);
              private_t0 = (private_jx * private_jx);
              private_t1 = (private_jy * private_jy);
              private_t2 = (private_jz * private_jz);
              private_e += (1.19000006F * ((((-11) * private_rho) + (19 * ((private_t0 + private_t1) + private_t2))) - private_e));
              private_eps += (1.39999998F * ((3 * private_rho) - private_eps));
              private_qx += (1.20000005F * ((((-2.F) / 3) * private_jx) - private_qx));
              private_qy += (1.20000005F * ((((-2.F) / 3) * private_jy) - private_qy));
              private_qz += (1.20000005F * ((((-2.F) / 3) * private_jz) - private_qz));
              private_px += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((((2 * private_t0) - private_t1) - private_t2) - private_px));
              private_pix += ((-1.39999998F) * private_pix);
              private_pw += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_t1 - private_t2) - private_pw));
              private_piw += ((-1.39999998F) * private_piw);
              private_pxy += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jx * private_jy) - private_pxy));
              private_pyz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jy * private_jz) - private_pyz));
              private_pxz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jz * private_jx) - private_pxz));
              private_mx += ((-1.98000002F) * private_mx);
              private_my += ((-1.98000002F) * private_my);
              private_mz += ((-1.98000002F) * private_mz);
              private_f0 = ((((1.F / 19) * private_rho) - ((5.F / 399) * private_e)) + ((1.F / 21) * private_eps));
              private_t0 = ((((1.F / 19) * private_rho) - ((11.F / 2394) * private_e)) - ((1.F / 63) * private_eps));
              private_t1 = (private_px - private_pix);
              private_t2 = (private_pw - private_piw);
              private_f1 = ((private_t0 + ((1.F / 18) * private_t1)) + ((1.F / 10) * (private_jx - private_qx)));
              private_f2 = ((private_t0 + ((1.F / 18) * private_t1)) - ((1.F / 10) * (private_jx - private_qx)));
              private_f3 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jy - private_qy)));
              private_f4 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jy - private_qy)));
              private_f5 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jz - private_qz)));
              private_f6 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jz - private_qz)));
              private_t0 = ((((1.F / 19) * private_rho) + ((4.F / 1197) * private_e)) + ((1.F / 252) * private_eps));
              private_t1 = (((1.F / 10) * private_jx) + ((1.F / 40) * private_qx));
              private_t2 = (((1.F / 10) * private_jy) + ((1.F / 40) * private_qy));
              private_t3 = (((1.F / 10) * private_jz) + ((1.F / 40) * private_qz));
              private_f7 = ((((((((private_t0 + private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx - private_my)));
              private_f8 = ((((((((private_t0 - private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx + private_my)));
              private_f9 = ((((((((private_t0 + private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx + private_my)));
              private_f10 = ((((((((private_t0 - private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx - private_my)));
              private_f11 = ((((((((private_t0 + private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx - private_mz)));
              private_f12 = ((((((((private_t0 - private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx + private_mz)));
              private_f13 = ((((((((private_t0 + private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx + private_mz)));
              private_f14 = ((((((((private_t0 - private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx - private_mz)));
              private_f15 = ((((((private_t0 + private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my - private_mz)));
              private_f16 = ((((((private_t0 - private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my + private_mz)));
              private_f17 = ((((((private_t0 + private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my + private_mz)));
              private_f18 = ((((((private_t0 - private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my - private_mz)));
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 0] = private_f0;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 1] = private_f1;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 2] = private_f2;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 3] = private_f3;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 4] = private_f4;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5] = private_f5;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6] = private_f6;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 7] = private_f7;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 8] = private_f8;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 9] = private_f9;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 10] = private_f10;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 11] = private_f11;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 12] = private_f12;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 13] = private_f13;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 14] = private_f14;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 15] = private_f15;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 16] = private_f16;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 17] = private_f17;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 18] = private_f18;
            }
      } else
        for (int c3 = 0; c3 <= 127; c3 += 16)
          for (int c5 = ppcg_max(t1_0, ((t0_0 + t1_0 + 4) % 4) - 16 * b1 + 63 * t0_0); c5 <= 15; c5 += 4)
            for (int c6 = t2_0; c6 <= 15; c6 += 4) {
              private_g = 0;
              if (t2_0 == 3 && c3 == 112 && c6 == 15) {
                private_g = (private_g | 5441);
              } else if (t2_0 == 0 && c3 == 0 && c6 == 0)
                private_g = (private_g | 10882);
              if (4 * b1 + t1_0 == 15 * t0_0 && 16 * b1 + c5 == 63 * t0_0) {
                private_g = (private_g | 164616);
              } else if (b1 == 7 && t0_0 == 0 && t1_0 == 3 && c5 == 15)
                private_g = (private_g | 82116);
              if (t0_0 == 0)
                private_g = (private_g | 208928);
              private_f0 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 0];
              if (private_g & 262143) {
                if (private_g & 2) {
                  private_f1 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 2];
                } else {
                  private_f1 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                }
                if (private_g & 1) {
                  private_f2 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 1];
                } else {
                  private_f2 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                }
                if (private_g & 8) {
                  private_f3 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 4];
                } else {
                  private_f3 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                }
                if (private_g & 4) {
                  private_f4 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 3];
                } else {
                  private_f4 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 4];
                }
                if (private_g & 32) {
                  private_f5 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6];
                } else {
                  private_f5 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5];
                }
                if (private_g & 16) {
                  private_f6 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5];
                } else {
                  private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6];
                }
                if (private_g & 512) {
                  private_f7 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 10];
                } else {
                  private_f7 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                }
                if (private_g & 256) {
                  private_f8 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 9];
                } else {
                  private_f8 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                }
                if (private_g & 128) {
                  private_f9 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 8];
                } else {
                  private_f9 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                }
                if (private_g & 64) {
                  private_f10 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 7];
                } else {
                  private_f10 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                }
                if (private_g & 8192) {
                  private_f11 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 14];
                } else {
                  private_f11 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                }
                if (private_g & 4096) {
                  private_f12 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 13];
                } else {
                  private_f12 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                }
                if (private_g & 2048) {
                  private_f13 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 12];
                } else {
                  private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                }
                if (private_g & 1024) {
                  private_f14 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 11];
                } else {
                  private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                }
                if (private_g & 131072) {
                  private_f15 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 18];
                } else {
                  private_f15 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                }
                if (private_g & 65536) {
                  private_f16 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 17];
                } else {
                  private_f16 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 16];
                }
                if (private_g & 32768) {
                  private_f17 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 16];
                } else {
                  private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                }
                if (private_g & 16384) {
                  private_f18 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 15];
                } else {
                  private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 18];
                }
                if ((0)) {
                  private_f13 += (0.400000006F / 6);
                  private_f14 -= (0.400000006F / 6);
                }
              } else {
                private_f1 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 1];
                private_f2 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 2];
                private_f3 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 3];
                private_f4 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 4];
                private_f5 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5];
                private_f6 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6];
                private_f7 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 7];
                private_f8 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6 + 1)) * 19 + 8];
                private_f9 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 9];
                private_f10 = Lattices[((((c0 % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6 + 1)) * 19 + 10];
                private_f11 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 11];
                private_f12 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 12];
                private_f13 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (-((-c3 - c6 + 4294967296) % 4294967296) + 4294967295)) * 19 + 13];
                private_f14 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6 + 1)) * 19 + 14];
                private_f15 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 15];
                private_f16 = Lattices[((((c0 % 2) * 128 + ((t0_0 + 4294967295) % 4294967296)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 16];
                private_f17 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + ((16 * b1 - 63 * t0_0 + c5 + 4294967295) % 4294967296)) * 128 + (c3 + c6)) * 19 + 17];
                private_f18 = Lattices[((((c0 % 2) * 128 + (t0_0 + 1)) * 128 + (16 * b1 - 63 * t0_0 + c5 + 1)) * 128 + (c3 + c6)) * 19 + 18];
              }
              private_t0 = (((((private_f1 + private_f2) + private_f3) + private_f4) + private_f5) + private_f6);
              private_t1 = (((((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) + private_f15) + private_f16) + private_f17) + private_f18);
              private_rho = ((private_f0 + private_t0) + private_t1);
              private_e = ((((-30) * private_f0) - (11 * private_t0)) + (8 * private_t1));
              private_eps = (((12 * private_f0) - (4 * private_t0)) + private_t1);
              private_t0 = (private_f1 - private_f2);
              private_t1 = (((((((private_f7 - private_f8) + private_f9) - private_f10) + private_f11) - private_f12) + private_f13) - private_f14);
              private_jx = (private_t0 + private_t1);
              private_qx = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f3 - private_f4);
              private_t1 = (((((((private_f7 + private_f8) - private_f9) - private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_jy = (private_t0 + private_t1);
              private_qy = (((-4) * private_t0) + private_t1);
              private_t0 = (private_f5 - private_f6);
              private_t1 = (((((((private_f11 + private_f12) - private_f13) - private_f14) + private_f15) + private_f16) - private_f17) - private_f18);
              private_jz = (private_t0 + private_t1);
              private_qz = (((-4) * private_t0) + private_t1);
              private_t0 = ((2 * (private_f1 + private_f2)) - (((private_f3 + private_f4) + private_f5) + private_f6));
              private_t1 = ((((((((private_f7 + private_f8) + private_f9) + private_f10) + private_f11) + private_f12) + private_f13) + private_f14) - (2 * (((private_f15 + private_f16) + private_f17) + private_f18)));
              private_px = (private_t0 + private_t1);
              private_pix = (((-2) * private_t0) + private_t1);
              private_t0 = ((private_f3 + private_f4) - (private_f5 + private_f6));
              private_t1 = ((((private_f7 + private_f8) + private_f9) + private_f10) - (((private_f11 + private_f12) + private_f13) + private_f14));
              private_pw = (private_t0 + private_t1);
              private_piw = (((-2) * private_t0) + private_t1);
              private_pxy = (((private_f7 - private_f8) - private_f9) + private_f10);
              private_pyz = (((private_f15 - private_f16) - private_f17) + private_f18);
              private_pxz = (((private_f11 - private_f12) - private_f13) + private_f14);
              private_mx = (((((((private_f7 - private_f8) + private_f9) - private_f10) - private_f11) + private_f12) - private_f13) + private_f14);
              private_my = ((((((((-private_f7) - private_f8) + private_f9) + private_f10) + private_f15) - private_f16) + private_f17) - private_f18);
              private_mz = (((((((private_f11 + private_f12) - private_f13) - private_f14) - private_f15) - private_f16) + private_f17) + private_f18);
              private_t0 = (private_jx * private_jx);
              private_t1 = (private_jy * private_jy);
              private_t2 = (private_jz * private_jz);
              private_e += (1.19000006F * ((((-11) * private_rho) + (19 * ((private_t0 + private_t1) + private_t2))) - private_e));
              private_eps += (1.39999998F * ((3 * private_rho) - private_eps));
              private_qx += (1.20000005F * ((((-2.F) / 3) * private_jx) - private_qx));
              private_qy += (1.20000005F * ((((-2.F) / 3) * private_jy) - private_qy));
              private_qz += (1.20000005F * ((((-2.F) / 3) * private_jz) - private_qz));
              private_px += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((((2 * private_t0) - private_t1) - private_t2) - private_px));
              private_pix += ((-1.39999998F) * private_pix);
              private_pw += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_t1 - private_t2) - private_pw));
              private_piw += ((-1.39999998F) * private_piw);
              private_pxy += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jx * private_jy) - private_pxy));
              private_pyz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jy * private_jz) - private_pyz));
              private_pxz += (((2 * 1000.) / (((6 * 0.400000006F) * 128) + 1000.)) * ((private_jz * private_jx) - private_pxz));
              private_mx += ((-1.98000002F) * private_mx);
              private_my += ((-1.98000002F) * private_my);
              private_mz += ((-1.98000002F) * private_mz);
              private_f0 = ((((1.F / 19) * private_rho) - ((5.F / 399) * private_e)) + ((1.F / 21) * private_eps));
              private_t0 = ((((1.F / 19) * private_rho) - ((11.F / 2394) * private_e)) - ((1.F / 63) * private_eps));
              private_t1 = (private_px - private_pix);
              private_t2 = (private_pw - private_piw);
              private_f1 = ((private_t0 + ((1.F / 18) * private_t1)) + ((1.F / 10) * (private_jx - private_qx)));
              private_f2 = ((private_t0 + ((1.F / 18) * private_t1)) - ((1.F / 10) * (private_jx - private_qx)));
              private_f3 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jy - private_qy)));
              private_f4 = (((private_t0 - ((1.F / 36) * private_t1)) + ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jy - private_qy)));
              private_f5 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) + ((1.F / 10) * (private_jz - private_qz)));
              private_f6 = (((private_t0 - ((1.F / 36) * private_t1)) - ((1.F / 12) * private_t2)) - ((1.F / 10) * (private_jz - private_qz)));
              private_t0 = ((((1.F / 19) * private_rho) + ((4.F / 1197) * private_e)) + ((1.F / 252) * private_eps));
              private_t1 = (((1.F / 10) * private_jx) + ((1.F / 40) * private_qx));
              private_t2 = (((1.F / 10) * private_jy) + ((1.F / 40) * private_qy));
              private_t3 = (((1.F / 10) * private_jz) + ((1.F / 40) * private_qz));
              private_f7 = ((((((((private_t0 + private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx - private_my)));
              private_f8 = ((((((((private_t0 - private_t1) + private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx + private_my)));
              private_f9 = ((((((((private_t0 + private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxy)) + ((1.F / 8) * (private_mx + private_my)));
              private_f10 = ((((((((private_t0 - private_t1) - private_t2) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) + ((1.F / 12) * private_pw)) + ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxy)) - ((1.F / 8) * (private_mx - private_my)));
              private_f11 = ((((((((private_t0 + private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx - private_mz)));
              private_f12 = ((((((((private_t0 - private_t1) + private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx + private_mz)));
              private_f13 = ((((((((private_t0 + private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) - ((1.F / 4) * private_pxz)) - ((1.F / 8) * (private_mx + private_mz)));
              private_f14 = ((((((((private_t0 - private_t1) - private_t3) + ((1.F / 36) * private_px)) + ((1.F / 72) * private_pix)) - ((1.F / 12) * private_pw)) - ((1.F / 24) * private_piw)) + ((1.F / 4) * private_pxz)) + ((1.F / 8) * (private_mx - private_mz)));
              private_f15 = ((((((private_t0 + private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my - private_mz)));
              private_f16 = ((((((private_t0 - private_t2) + private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my + private_mz)));
              private_f17 = ((((((private_t0 + private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) - ((1.F / 4) * private_pyz)) + ((1.F / 8) * (private_my + private_mz)));
              private_f18 = ((((((private_t0 - private_t2) - private_t3) - ((1.F / 18) * private_px)) - ((1.F / 36) * private_pix)) + ((1.F / 4) * private_pyz)) - ((1.F / 8) * (private_my - private_mz)));
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 0] = private_f0;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 1] = private_f1;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 2] = private_f2;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 3] = private_f3;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 4] = private_f4;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 5] = private_f5;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 6] = private_f6;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 7] = private_f7;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 8] = private_f8;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 9] = private_f9;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 10] = private_f10;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 11] = private_f11;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 12] = private_f12;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 13] = private_f13;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 14] = private_f14;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 15] = private_f15;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 16] = private_f16;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 17] = private_f17;
              Lattices[(((((c0 + 1) % 2) * 128 + t0_0) * 128 + (16 * b1 - 63 * t0_0 + c5)) * 128 + (c3 + c6)) * 19 + 18] = private_f18;
            }
    }
}
