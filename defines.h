#ifndef EEPROMFIELD_H
#define EEPROMFIELD_H

/*******************************************************************************
 * 定义产品规格、布局；
 * 请根据实际需求进行修改
 ******************************************************************************/

/*
 * 使用的CAN接口信息定义
 * 注意：如果使用多个CAN接口，请自行定义、Open/Close
 */
#define CAN1_DEV                0
#define CAN1_NODE               0x10
#define CAN1_BAUD               250000
#define CAN1_EDS                "/apps/can0.eds"

#define CAN2_DEV                1
#define CAN2_NODE               0x1234
#define CAN2_BAUD               250000
#define CAN2_EDS                "/apps/can1.eds"

#define BUTTONBAR_WIDTH 121
#define BUTTONBAR_HEIGHT 720


/*
 * 定义变量在EEPROM的偏移和大小
 * 可用空间: 0～eeprom_size()
 * ICM7100的空间约31KB
 * 注意：请根据需求自行修改
 */
#define EEPROM_LANGUAGE_OFFSET  2       // 多语言
#define EEPROM_LANGUAGE_SIZE    1       // 多语言所占的字节数
#define EEPROM_LIGHTSETTING_OFFSET  3   //灯光设置
#define EEPROM_LIGHTSETTING_SIZE    1

/*
 * CAN发送PDO列表
 * 注意：请根据需求自行修改
 */
enum CanPdoIndex {
    CanPdoInquireError = 0,

    CanPdoTBD
};

/*
 * CAN字典变量列表
 * 注意：请根据需求自行修改
 */
enum CanVarName {
    // Receive PDO[1] 0x4FA0001
    C1_R04FA0001B1           = 0,
    C1_R04FA0001B2           = 1,
    C1_R04FA0001B3           = 2,
    C1_R04FA0001B4           = 3,
    C1_R04FA0001B56          = 4,
    C1_R04FA0001B78          = 5,
    // Receive PDO[2] 0x4FA0002
    C1_R04FA0002B12          = 6,
    C1_R04FA0002B34          = 7,
    C1_R04FA0002B56          = 8,
    C1_R04FA0002B78          = 9,
    // Receive PDO[3] 0x4FA0007
    C1_R04FA0007B1           = 10,
    C1_R04FA0007B2           = 11,
    C1_R04FA0007B3           = 12,
    C1_R04FA0007B4           = 13,
    C1_R04FA0007B5           = 14,
    C1_R04FA0007B6           = 15,
    C1_R04FA0007B7           = 16,
    C1_R04FA0007B8           = 17,
    // Receive PDO[4] 0x4FA0008
    C1_R04FA0008B1           = 18,
    C1_R04FA0008B2           = 19,
    C1_R04FA0008B3           = 20,
    C1_R04FA0008B4           = 21,
    C1_R04FA0008B5           = 22,
    C1_R04FA0008B6           = 23,
    C1_R04FA0008B7           = 24,
    C1_R04FA0008B8           = 25,
    // Receive PDO[5] 0x4FA0009
    C1_R04FA0009B1           = 26,
    C1_R04FA0009B2           = 27,
    C1_R04FA0009B3           = 28,
    C1_R04FA0009B4           = 29,
    C1_R04FA0009B5           = 30,
    C1_R04FA0009B6           = 31,
    C1_R04FA0009B7           = 32,
    C1_R04FA0009B8           = 33,
    // Receive PDO[6] 0x4FA0010
    C1_R04FA0010B1           = 34,
    C1_R04FA0010B2           = 35,
    C1_R04FA0010B3           = 36,
    C1_R04FA0010B4           = 37,
    C1_R04FA0010B5           = 38,
    C1_R04FA0010B6           = 39,
    C1_R04FA0010B7           = 40,
    C1_R04FA0010B8           = 41,
    // Receive PDO[7] 0x4FA0011
    C1_R04FA0011B12          = 42,
    C1_R04FA0011B34          = 43,
    C1_R04FA0011B56          = 44,
    C1_R04FA0011B78          = 45,
    // Receive PDO[8] 0x4FA0012
    C1_R04FA0012B12          = 46,
    C1_R04FA0012B34          = 47,
    C1_R04FA0012B56          = 48,
    C1_R04FA0012B78          = 49,
    // Receive PDO[9] 0x4FB0004
    C1_R04FB0004B1           = 50,
    C1_R04FB0004B2           = 51,
    C1_R04FB0004B3           = 52,
    C1_R04FB0004B4           = 53,
    C1_R04FB0004B5           = 54,
    C1_R04FB0004B6           = 55,
    C1_R04FB0004B7           = 56,
    C1_R04FB0004B8           = 57,
    // Receive PDO[10] 0x4FB0006
    C1_R04FB0006B1           = 58,
    C1_R04FB0006B2           = 59,
    C1_R04FB0006B3           = 60,
    C1_R04FB0006B4           = 61,
    C1_R04FB0006B5           = 62,
    C1_R04FB0006B6           = 63,
    C1_R04FB0006B7           = 64,
    C1_R04FB0006B8           = 65,
    // Receive PDO[11] 0x4FB0010
    C1_R04FB0010B12          = 66,
    C1_R04FB0010B34          = 67,
    C1_R04FB0010B56          = 68,
    C1_R04FB0010B78          = 69,
    // Receive PDO[12] 0xCF00400
    C1_R0CF00400B1           = 70,
    C1_R0CF00400B23          = 71,
    C1_R0CF00400B45          = 72,
    C1_R0CF00400B67          = 73,
    C1_R0CF00400B8           = 74,
    // Receive PDO[13] 0x18FEE500
    C1_R18FEE500B1234        = 75,
    C1_R18FEE500B5678        = 76,
    // Receive PDO[14] 0x4FA0003
    C1_R04FA0003B1234        = 77,
    C1_R04FA0003B5678        = 78,
    // Receive PDO[15] 0x4FA0022
    C1_R04FA0022B1           = 79,
    // Receive PDO[16] 0x4FC0001
    C1_R04FC0001B12          = 80,
    C1_R04FC0001B34          = 81,
    C1_R04FC0001B5           = 82,
    C1_R04FC0001B6           = 83,
    C1_R04FC0001B7           = 84,
    C1_R04FC0001B8           = 85,
    // Receive PDO[17] 0x4FB0001
    C1_R04FB0001B12          = 86,
    C1_R04FB0001B34          = 87,
    C1_R04FB0001B56          = 88,
    C1_R04FB0001B78          = 89,
    // Receive PDO[18] 0x4FB0002
    C1_R04FB0002B12          = 90,
    C1_R04FB0002B34          = 91,
    C1_R04FB0002B56          = 92,
    C1_R04FB0002B78          = 93,
    // Receive PDO[19] 0x4FA0004
    C1_R04FA0004B1234        = 94,
    C1_R04FA0004B5           = 95,
    C1_R04FA0004B6           = 96,
    C1_R04FA0004B7           = 97,
    C1_R04FA0004B8           = 98,
    // Receive PDO[20] 0x4FB0005
    C1_R04FB0005B1           = 99,
    C1_R04FB0005B2           = 100,
    C1_R04FB0005B3           = 101,
    C1_R04FB0005B4           = 102,
    C1_R04FB0005B5           = 103,
    C1_R04FB0005B6           = 104,
    C1_R04FB0005B7           = 105,
    C1_R04FB0005B8           = 106,
    // Receive PDO[21] 0x18F00503
    C1_R18F00503B1           = 107,
    C1_R18F00503B2           = 108,
    C1_R18F00503B3           = 109,
    C1_R18F00503B4           = 110,
    // Receive PDO[22] 0x18FE08F4
    C1_R18FE08F4B1           = 111,
    C1_R18FE08F4B2           = 112,
    C1_R18FE08F4B3           = 113,
    C1_R18FE08F4B4           = 114,
    // Receive PDO[23] 0x18FE0FF6
    C1_R18FE0FF6B1           = 115,
    C1_R18FE0FF6B2           = 116,
    C1_R18FE0FF6B34          = 117,
    C1_R18FE0FF6B5           = 118,
    // Receive PDO[24] 0x18FE0FF7
    C1_R18FE0FF7B1           = 119,
    C1_R18FE0FF7B2           = 120,
    C1_R18FE0FF7B3           = 121,
    C1_R18FE0FF7B4           = 122,
    C1_R18FE0FF7B5           = 123,
    C1_R18FE0FF7B6           = 124,
    C1_R18FE0FF7B7           = 125,
    C1_R18FE0FF7B8           = 126,
    // Receive PDO[25] 0x4FA0014
    C1_R04FA0014B1           = 127,
    C1_R04FA0014B2           = 128,
    C1_R04FA0014B3           = 129,
    C1_R04FA0014B4           = 130,
    C1_R04FA0014B5           = 131,
    C1_R04FA0014B6           = 132,
    C1_R04FA0014B78          = 133,
    // Receive PDO[26] 0x500
    C1_R500B1                = 134,
    // Receive PDO[27] 0x18FF0507
    C1_R18ff0507B12          = 135,
    C1_R18ff0507B34          = 136,
    C1_R18ff0507B56          = 137,
    C1_R18ff0507B78          = 138,
    // Receive PDO[28] 0x18FF0607
    C1_R18ff0607B1           = 139,
    C1_R18ff0607B2           = 140,
    C1_R18ff0607B3           = 141,
    // Receive PDO[29] 0x18FF0707
    C1_R18ff0707B12          = 142,
    C1_R18ff0707B34          = 143,
    C1_R18ff0707B56          = 144,
    C1_R18ff0707B78          = 145,
    // Receive PDO[30] 0x18FF0807
    C1_R18ff0807B12          = 146,
    C1_R18ff0807B34          = 147,
    C1_R18ff0807B5           = 148,
    C1_R18ff0807B6           = 149,
    C1_R18ff0807B78          = 150,
    // Receive PDO[31] 0x18FF0907
    C1_R18ff0907B12          = 151,
    C1_R18ff0907B34          = 152,
    C1_R18ff0907B56          = 153,
    C1_R18ff0907B78          = 154,
    // Receive PDO[32] 0x4FA0013
    C1_R04FA0013B18          = 155,
    // Receive PDO[33] 0x18FF0407
    C1_R18FF0407B12          = 156,
    C1_R18FF0407B34          = 157,
    C1_R18FF0407B56          = 158,
    C1_R18FF0407B78          = 159,
    // Receive PDO[34] 0x18FECA00
    C1_R18FECA00B18          = 160,
    // Receive PDO[35] 0x18ECFF00
    C1_R18ECFF00B18          = 161,
    // Receive PDO[36] 0x18EBFF00
    C1_R18EBFF00B18          = 162,
    // Receive PDO[37] 0x18FE01F6
    C1_R18FE01F6B1           = 163,
    C1_R18FE01F6B2           = 164,
    C1_R18FE01F6B3           = 165,
    C1_R18FE01F6B4           = 166,
    C1_R18FE01F6B5           = 167,
    C1_R18FE01F6B6           = 168,
    C1_R18FE01F6B7           = 169,
    C1_R18FE01F6B8           = 170,
    // Receive PDO[38] 0x18FF0000
    C1_R18FF0000B1           = 171,
    C1_R18FF0000B2           = 172,
    C1_R18FF0000B3           = 173,
    C1_R18FF0000B4           = 174,
    C1_R18FF0000B5           = 175,
    C1_R18FF0000B6           = 176,
    C1_R18FF0000B7           = 177,
    C1_R18FF0000B8           = 178,
    // Send PDO[1] 0x4FA0001
    C1_T04FB0012B12          = 179,
    C1_T04FB0012B34          = 180,
    C1_T04FB0012B56          = 181,
    C1_T04FB0012B78          = 182,
    // Send PDO[2] 0x4FA0002
    C1_T281B12               = 183,
    C1_T281B3                = 184,
    C1_T281B4                = 185,
    C1_T281B5                = 186,
    C1_T281B6                = 187,
    C1_T281B7                = 188,
    C1_T281B8                = 189,

    CanVarNum,
};


#endif // EEPROMFIELD_H
