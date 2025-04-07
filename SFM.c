// === PIN AYARLARI ===
#define SFM_ONOFF      LATC.F4
#define SFM_ONOFF_TRIS TRISC.F4

// === GEREKLI DEGISKENLER ===
char txt[10];
unsigned char i;

// === I2C2 Baslat ===
void I2C2_Init_Config() {
    // RF4 = SDA2, RF5 = SCL2
    PPS_Mapping(100, _INPUT,  _SDI1);     // SDA (IN)
    PPS_Mapping(100, _OUTPUT, _SDO1);     // SDA (OUT)
    PPS_Mapping(101, _OUTPUT, _SCK1OUT);  // SCL (OUT)
    // SCL (IN) genelde otomatik tanimlanir

    I2C2_Init(100000);   // 100kHz I2C baslat
    Delay_ms(100);
}


// === UART3 Baslat ===
void UART3_Init_Config() {
    PPS_Mapping(69, _INPUT,  _U3RX);   // RX = RP69
    PPS_Mapping(68, _OUTPUT, _U3TX);   // TX = RP68
    UART3_Init(9600);
    Delay_ms(100);
}

// === SFM ON/OFF pini ayarla ===
void SFM_PowerOn() {
    SFM_ONOFF_TRIS = 0;
    SFM_ONOFF = 1;      // Sensöre güç ver
    UART3_Write_Text("SFM_ONOFF: HIGH (aktif)\r\n");
}

// === Cihaz Taramasi ===
void I2C2_ScanDevices() {
    UART3_Write_Text("I2C TARAMA BASLIYOR...\r\n");

    for (i = 0x08; i <= 0x77; i++) {
        I2C2_Start();
        if (I2C2_Write(i << 1) == 0) {
            UART3_Write_Text("I2C cihaz bulundu: 0x");
            ByteToHex(i, txt);
            UART3_Write_Text(txt);
            UART3_Write_Text("\r\n");
        }
        I2C2_Stop();
        Delay_ms(10);
    }

    UART3_Write_Text("I2C TARAMA BITTI.\r\n");
}

// === ANA PROGRAM ===
void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    UART3_Init_Config();
    I2C2_Init_Config();

    UART3_Write_Text(">>> SFM I2C Cihaz Taramasi <<<\r\n");

    SFM_PowerOn();
    Delay_ms(100); // sensör açilmasini bekle

    I2C2_ScanDevices();

    while (1);
}
