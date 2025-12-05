u32 log_2(u32 x) {
  if(x < 0x00008000) {
    if(x < 0x00000080) {
      if(x < 0x00000008) {
        if(x < 0x00000002) {
          if(x < 0x00000001) {
            return 0;
          } else {
            return 1;
          }
        } else {
          if(x < 0x00000004) {
            return 2;
          } else {
            return 3;
          }
        }
      } else {
        if(x < 0x00000020) {
          if(x < 0x00000010) {
            return 4;
          } else {
            return 5;
          }
        } else {
          if(x < 0x00000040) {
            return 6;
          } else {
            return 7;
          }
        }
      }
    } else {
      if(x < 0x00000800) {
        if(x < 0x00000200) {
          if(x < 0x00000100) {
            return 8;
          } else {
            return 9;
          }
        } else {
          if(x < 0x00000400) {
            return 10;
          } else {
            return 11;
          }
        }
      } else {
        if(x < 0x00002000) {
          if(x < 0x00001000) {
            return 12;
          } else {
            return 13;
          }
        } else {
          if(x < 0x00004000) {
            return 14;
          } else {
            return 15;
          }
        }
      }
    }
  } else {
    if(x < 0x00800000) {
      if(x < 0x00080000) {
        if(x < 0x00020000) {
          if(x < 0x00010000) {
            return 16;
          } else {
            return 17;
          }
        } else {
          if(x < 0x00040000) {
            return 18;
          } else {
            return 19;
          }
        }
      } else {
        if(x < 0x00200000) {
          if(x < 0x00100000) {
            return 20;
          } else {
            return 21;
          }
        } else {
          if(x < 0x00400000) {
            return 22;
          } else {
            return 23;
          }
        }
      }
    } else {
      if(x < 0x08000000) {
        if(x < 0x02000000) {
          if(x < 0x01000000) {
            return 24;
          } else {
            return 25;
          }
        } else {
          if(x < 0x04000000) {
            return 26;
          } else {
            return 27;
          }
        }
      } else {
        if(x < 0x20000000) {
          if(x < 0x10000000) {
            return 28;
          } else {
            return 29;
          }
        } else {
          if(x < 0x40000000) {
            return 30;
          } else {
            return 31;
          }
        }
      }
    }
  }
}
