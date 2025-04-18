/*

Bu satırı görmek = ✅ SPI sorunsuz
✅ WREN başarılı
✅ 3-byte adres doğru
✅ EEPROM yazma ve okuma %100 tamamlandı



*/




// === SABİTLER ===
#define EEPROM_CMD_WREN   0x06
#define EEPROM_CMD_WRITE  0x02
#define EEPROM_CMD_READ   0x03
#define EEPROM_CMD_RDSR   0x05

// === EEPROM PIN TANIMLARI ===
#define EEPROM_CS        LATF.F13
#define EEPROM_CS_TRIS   TRISF.F13

// === STATUS REGISTER OKUMA ===
unsigned char EEPROM_ReadStatus() {
    unsigned char status;
    EEPROM_CS = 0;
    SPI1_Write(EEPROM_CMD_RDSR);
    status = SPI1_Read(0xFF);
    EEPROM_CS = 1;
    return status;
}

// === YAZMA İZNİ (WREN) ===
void EEPROM_WriteEnable() {
    EEPROM_CS = 0;
    SPI1_Write(EEPROM_CMD_WREN);
    EEPROM_CS = 1;
    Delay_ms(1);
}

// === SPI PPS AYARI ===
void SPI1_PPS_Init() {
    PPS_Mapping(96, _OUTPUT, _SCK1OUT);  // RF0
    PPS_Mapping(97, _OUTPUT, _SDO1);     // RF1
    PPS_Mapping(98, _INPUT,  _SDI1);     // RF2

    TRISF.F0 = 0; // CLK
    TRISF.F1 = 0; // MOSI
    TRISF.F2 = 1; // MISO
    EEPROM_CS_TRIS = 0;
    EEPROM_CS = 1;
}

// === SPI BAŞLAT ===
void SPI1_Initialize_Master() {
    SPI1_Init();
}

// === UART3 BAŞLAT ===
void UART3_Init_Config(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT,  _U3RX);     // RX = RP69
    PPS_Mapping(68, _OUTPUT, _U3TX);     // TX = RP68
    UART3_Init(baudrate);
    Delay_ms(100);
}

// === ANA PROGRAM ===
void main() {
    unsigned char status, okunan;
    char txt[10];

    // Sistem saat ayarı (kesin olmalı)
    PLLFBD = 70;
    CLKDIV = 0x0000;

    // Başlatmalar
    SPI1_PPS_Init();
    SPI1_Initialize_Master();
    UART3_Init_Config(9600);

    UART3_Write_Text("\r\n>>> EEPROM TAM TEST (25CSM04) <<<\r\n");

    // Yazma izni ver
    UART3_Write_Text("WREN gönderiliyor...\r\n");
    EEPROM_WriteEnable();

    // Status register oku
    status = EEPROM_ReadStatus();
    UART3_Write_Text("Status Register: ");
    ByteToHex(status, txt);
    UART3_Write_Text(txt);
    UART3_Write_Text("\r\n");

    if ((status & 0x02) == 0x02)
        UART3_Write_Text("WEL biti = 1 ✅\r\n");
    else {
        UART3_Write_Text("WEL biti = 0 ❌ Yazma iptal!\r\n");
        while(1);
    }

    // EEPROM’a 1 byte (A) yaz
    UART3_Write_Text("EEPROM’a 0x41 ('A') yaziliyor...\r\n");
    EEPROM_CS = 0;
    SPI1_Write(EEPROM_CMD_WRITE);
    SPI1_Write(0x00);  // Adres byte 1 (MSB)
    SPI1_Write(0x00);  // Adres byte 2
    SPI1_Write(0x00);  // Adres byte 3 (LSB)
    SPI1_Write('A');   // Veri: 0x41
    EEPROM_CS = 1;
    Delay_ms(20);

    // EEPROM’dan oku
    UART3_Write_Text("EEPROM’dan okuma yapiliyor...\r\n");
    EEPROM_CS = 0;
    SPI1_Write(EEPROM_CMD_READ);
    SPI1_Write(0x00);  // Adres byte 1
    SPI1_Write(0x00);  // Adres byte 2
    SPI1_Write(0x00);  // Adres byte 3
    okunan = SPI1_Read(0xFF);
    EEPROM_CS = 1;

    // Sonucu UART ile gönder
    UART3_Write_Text("Okunan byte: ");
    ByteToHex(okunan, txt);
    UART3_Write_Text(txt);
    UART3_Write_Text(" (");
    UART3_Write(okunan);
    UART3_Write_Text(")\r\n");

    while(1);
}
