#ifndef __HW_TEST_H
#define __HW_TEST_H	

void HW_TEST_INIT(void);
void HW_TEST(void);


//INPUT
#define IN1     PAin(3)
#define IN2     PAin(4)
#define IN3     PCin(4)
#define IN4     PCin(5)
#define IN5     PBin(0)
#define IN6     PBin(1)
#define IN7     PBin(2)
#define IN8     PEin(7)
#define IN9     PEin(8)
#define IN10     PEin(9)
#define IN11     PEin(10)
#define IN12     PEin(11)
#define IN13     PEin(12)
#define IN14     PEin(13)
#define IN15     PEin(14)
#define IN16     PEin(15)


//OUTPUT
#define GRL1     PEout(2)
#define GRL2     PEout(3)
#define GRL3     PEout(4)
#define GRL4     PEout(5)
#define GRL5     PEout(6)
#define GRL6     PBout(8)
#define GRL7     PBout(7)
#define GRL8     PBout(4)
#define GRL9     PBout(3)

#define GSFR1     PDout(7)
#define GSFR2     PDout(4)
#define GSFR3     PDout(3)
#define GSFR4     PDout(2)

#define TRANS_CTRL1     PAout(15)
#define TRANS_CTRL2     PCout(12)



#endif