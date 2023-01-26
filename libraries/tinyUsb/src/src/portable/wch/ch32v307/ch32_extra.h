/**
  Minimalistic copy / paste of ch32 structure used
*/
#pragma once
#define PERIPH_BASE           ((uint32_t)0x40000000) /* Peripheral base address in the alias region */
#define APB1PERIPH_BASE       (PERIPH_BASE)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)


#define USBHS_BASE            (AHBPERIPH_BASE + 0x3400)

typedef struct
{
  __IO uint8_t  CONTROL;
  __IO uint8_t  HOST_CTRL;
  __IO uint8_t  INT_EN;
  __IO uint8_t  DEV_AD;
  __IO uint16_t FRAME_NO;
  __IO uint8_t  SUSPEND;
  __IO uint8_t  RESERVED0;
  __IO uint8_t  SPEED_TYPE;
  __IO uint8_t  MIS_ST;
  __IO uint8_t  INT_FG;
  __IO uint8_t  INT_ST;
  __IO uint16_t RX_LEN;
  __IO uint16_t RESERVED1;
  __IO uint32_t ENDP_CONFIG;
  __IO uint32_t ENDP_TYPE;
  __IO uint32_t BUF_MODE;
  __IO uint32_t UEP0_DMA;               
  __IO uint32_t UEP1_RX_DMA;       
  __IO uint32_t UEP2_RX_DMA;       
  __IO uint32_t UEP3_RX_DMA;       
  __IO uint32_t UEP4_RX_DMA;       
  __IO uint32_t UEP5_RX_DMA;       
  __IO uint32_t UEP6_RX_DMA;       
  __IO uint32_t UEP7_RX_DMA;       
  __IO uint32_t UEP8_RX_DMA;      
  __IO uint32_t UEP9_RX_DMA;       
  __IO uint32_t UEP10_RX_DMA;      
  __IO uint32_t UEP11_RX_DMA;      
  __IO uint32_t UEP12_RX_DMA;      
  __IO uint32_t UEP13_RX_DMA;      
  __IO uint32_t UEP14_RX_DMA;      
  __IO uint32_t UEP15_RX_DMA;      
  __IO uint32_t UEP1_TX_DMA;       
  __IO uint32_t UEP2_TX_DMA;       
  __IO uint32_t UEP3_TX_DMA;       
  __IO uint32_t UEP4_TX_DMA;       
  __IO uint32_t UEP5_TX_DMA;      
  __IO uint32_t UEP6_TX_DMA;      
  __IO uint32_t UEP7_TX_DMA;       
  __IO uint32_t UEP8_TX_DMA;      
  __IO uint32_t UEP9_TX_DMA;       
  __IO uint32_t UEP10_TX_DMA;      
  __IO uint32_t UEP11_TX_DMA;      
  __IO uint32_t UEP12_TX_DMA;    
  __IO uint32_t UEP13_TX_DMA;      
  __IO uint32_t UEP14_TX_DMA;     
  __IO uint32_t UEP15_TX_DMA;      
  __IO uint16_t UEP0_MAX_LEN;
  __IO uint16_t RESERVED2;
  __IO uint16_t UEP1_MAX_LEN;
  __IO uint16_t RESERVED3;
  __IO uint16_t UEP2_MAX_LEN;
  __IO uint16_t RESERVED4;
  __IO uint16_t UEP3_MAX_LEN;
  __IO uint16_t RESERVED5;
  __IO uint16_t UEP4_MAX_LEN;
  __IO uint16_t RESERVED6;
  __IO uint16_t UEP5_MAX_LEN;
  __IO uint16_t RESERVED7;
  __IO uint16_t UEP6_MAX_LEN;
  __IO uint16_t RESERVED8;
  __IO uint16_t UEP7_MAX_LEN;
  __IO uint16_t RESERVED9;
  __IO uint16_t UEP8_MAX_LEN;
  __IO uint16_t RESERVED10;
  __IO uint16_t UEP9_MAX_LEN;
  __IO uint16_t RESERVED11;
  __IO uint16_t UEP10_MAX_LEN;
  __IO uint16_t RESERVED12;
  __IO uint16_t UEP11_MAX_LEN;
  __IO uint16_t RESERVED13;
  __IO uint16_t UEP12_MAX_LEN;
  __IO uint16_t RESERVED14;
  __IO uint16_t UEP13_MAX_LEN;
  __IO uint16_t RESERVED15;
  __IO uint16_t UEP14_MAX_LEN;
  __IO uint16_t RESERVED16;
  __IO uint16_t UEP15_MAX_LEN;
  __IO uint16_t RESERVED17;
  __IO uint16_t UEP0_TX_LEN;
  __IO uint8_t  UEP0_TX_CTRL;
  __IO uint8_t  UEP0_RX_CTRL;
  __IO uint16_t UEP1_TX_LEN;
  __IO uint8_t  UEP1_TX_CTRL;
  __IO uint8_t  UEP1_RX_CTRL;
  __IO uint16_t UEP2_TX_LEN;
  __IO uint8_t  UEP2_TX_CTRL;
  __IO uint8_t  UEP2_RX_CTRL;
  __IO uint16_t UEP3_TX_LEN;
  __IO uint8_t  UEP3_TX_CTRL;
  __IO uint8_t  UEP3_RX_CTRL;
  __IO uint16_t UEP4_TX_LEN;
  __IO uint8_t  UEP4_TX_CTRL;
  __IO uint8_t  UEP4_RX_CTRL;
  __IO uint16_t UEP5_TX_LEN;
  __IO uint8_t  UEP5_TX_CTRL;
  __IO uint8_t  UEP5_RX_CTRL;
  __IO uint16_t UEP6_TX_LEN;
  __IO uint8_t  UEP6_TX_CTRL;
  __IO uint8_t  UEP6_RX_CTRL;
  __IO uint16_t UEP7_TX_LEN;
  __IO uint8_t  UEP7_TX_CTRL;
  __IO uint8_t  UEP7_RX_CTRL;
  __IO uint16_t UEP8_TX_LEN;
  __IO uint8_t  UEP8_TX_CTRL;
  __IO uint8_t  UEP8_RX_CTRL;
  __IO uint16_t UEP9_TX_LEN;
  __IO uint8_t  UEP9_TX_CTRL;
  __IO uint8_t  UEP9_RX_CTRL;
  __IO uint16_t UEP10_TX_LEN;
  __IO uint8_t  UEP10_TX_CTRL;
  __IO uint8_t  UEP10_RX_CTRL;
  __IO uint16_t UEP11_TX_LEN;
  __IO uint8_t  UEP11_TX_CTRL;
  __IO uint8_t  UEP11_RX_CTRL;
  __IO uint16_t UEP12_TX_LEN;
  __IO uint8_t  UEP12_TX_CTRL;
  __IO uint8_t  UEP12_RX_CTRL;
  __IO uint16_t UEP13_TX_LEN;
  __IO uint8_t  UEP13_TX_CTRL;
  __IO uint8_t  UEP13_RX_CTRL;
  __IO uint16_t UEP14_TX_LEN;
  __IO uint8_t  UEP14_TX_CTRL;
  __IO uint8_t  UEP14_RX_CTRL;
  __IO uint16_t UEP15_TX_LEN;
  __IO uint8_t  UEP15_TX_CTRL;
  __IO uint8_t  UEP15_RX_CTRL;
} USBHSD_TypeDef;

typedef struct  __attribute__((packed))
{
    __IO uint8_t  CONTROL;
    __IO uint8_t  HOST_CTRL;
    __IO uint8_t  INT_EN;
    __IO uint8_t  DEV_AD;
    __IO uint16_t FRAME_NO;
    __IO uint8_t  SUSPEND;
    __IO uint8_t  RESERVED0;
    __IO uint8_t  SPEED_TYPE;
    __IO uint8_t  MIS_ST;
    __IO uint8_t  INT_FG;
    __IO uint8_t  INT_ST;
    __IO uint16_t RX_LEN;
    __IO uint16_t RESERVED1;
    __IO uint32_t HOST_EP_CONFIG;
    __IO uint32_t HOST_EP_TYPE;
    __IO uint32_t RESERVED2;
    __IO uint32_t RESERVED3;
    __IO uint32_t RESERVED4;
    __IO uint32_t HOST_RX_DMA;
    __IO uint32_t RESERVED5;
    __IO uint32_t RESERVED6;
    __IO uint32_t RESERVED7;
    __IO uint32_t RESERVED8;
    __IO uint32_t RESERVED9;
    __IO uint32_t RESERVED10;
    __IO uint32_t RESERVED11;
    __IO uint32_t RESERVED12;
    __IO uint32_t RESERVED13;
    __IO uint32_t RESERVED14;
    __IO uint32_t RESERVED15;
    __IO uint32_t RESERVED16;
    __IO uint32_t RESERVED17;
    __IO uint32_t RESERVED18;
    __IO uint32_t RESERVED19;
    __IO uint32_t HOST_TX_DMA;
    __IO uint32_t RESERVED20;
    __IO uint32_t RESERVED21;
    __IO uint32_t RESERVED22;
    __IO uint32_t RESERVED23;
    __IO uint32_t RESERVED24;
    __IO uint32_t RESERVED25;
    __IO uint32_t RESERVED26;
    __IO uint32_t RESERVED27;
    __IO uint32_t RESERVED28;
    __IO uint32_t RESERVED29;
    __IO uint32_t RESERVED30;
    __IO uint32_t RESERVED31;
    __IO uint32_t RESERVED32;
    __IO uint32_t RESERVED33;
    __IO uint16_t HOST_RX_MAX_LEN;
    __IO uint16_t RESERVED34;
    __IO uint32_t RESERVED35;
    __IO uint32_t RESERVED36;
    __IO uint32_t RESERVED37;
    __IO uint32_t RESERVED38;
    __IO uint32_t RESERVED39;
    __IO uint32_t RESERVED40;
    __IO uint32_t RESERVED41;
    __IO uint32_t RESERVED42;
    __IO uint32_t RESERVED43;
    __IO uint32_t RESERVED44;
    __IO uint32_t RESERVED45;
    __IO uint32_t RESERVED46;
    __IO uint32_t RESERVED47;
    __IO uint32_t RESERVED48;
    __IO uint32_t RESERVED49;
    __IO uint8_t  HOST_EP_PID;
    __IO uint8_t  RESERVED50;
    __IO uint8_t  RESERVED51;
    __IO uint8_t  HOST_RX_CTRL;
    __IO uint16_t HOST_TX_LEN;
    __IO uint8_t  HOST_TX_CTRL;
    __IO uint8_t  RESERVED52;
    __IO uint16_t HOST_SPLIT_DATA;
} USBHSH_TypeDef;


#define USBHSD              ((USBHSD_TypeDef *) USBHS_BASE)
#define USBHSH              ((USBHSH_TypeDef *) USBHS_BASE)

