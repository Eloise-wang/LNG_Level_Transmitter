void GCU1_Message(unsigned char* buff)
{
    unsigned int rData;

    /* packing signal ResidualFluidVolume_523697*/
    rData = (unsigned int)(ResidualFluidVolume_523697 - (0));
    buff[6u]  = ((unsigned char)(rData << 4u)) & 0xF0u;
    buff[7u]  = ((unsigned char)(rData >> 4u));

    /* packing signal GasCylinderEffectVolume_523696*/
    rData = (unsigned int)(GasCylinderEffectVolume_523696 - (0));
    buff[5u]  = ((unsigned char)(rData << 0u)) & 0xFFu;
    buff[6u] |= ((unsigned char)(rData >> 8u)) & 0x0Fu;

    /* packing signal GasCylinderPresNorValue_523695*/
    rData = (unsigned int)(GasCylinderPresNorValue_523695 - (0));
    buff[4u]  = ((unsigned char)(rData << 0u)) & 0xFFu;

    /* packing signal BufTankPresNorValue_523694*/
    rData = (unsigned int)(BufTankPresNorValue_523694 - (0));
    buff[3u]  = ((unsigned char)(rData << 0u)) & 0xFFu;

    /* packing signal Reserve3*/
    rData = (unsigned int)(Reserve3 - (0));
    buff[2u]  = ((unsigned char)(rData << 6u)) & 0xC0u;

    /* packing signal LiquidLevelSelfTSt_523700*/
    rData = (unsigned int)(LiquidLevelSelfTSt_523700 - (0));
    buff[2u] |= ((unsigned char)(rData << 4u)) & 0x30u;

    /* packing signal GasCylinderPresSelfTSt_523699*/
    rData = (unsigned int)(GasCylinderPresSelfTSt_523699 - (0));
    buff[2u] |= ((unsigned char)(rData << 2u)) & 0x0Cu;

    /* packing signal BufTankPresSelfTeSt_523698*/
    rData = (unsigned int)(BufTankPresSelfTeSt_523698 - (0));
    buff[2u] |= ((unsigned char)(rData << 0u)) & 0x03u;

    /* packing signal Reserve2*/
    rData = (unsigned int)(Reserve2 - (0));
    buff[1u]  = ((unsigned char)(rData << 6u)) & 0xC0u;

    /* packing signal LiquidLevelMeasSt_523691*/
    rData = (unsigned int)(LiquidLevelMeasSt_523691 - (0));
    buff[1u] |= ((unsigned char)(rData << 4u)) & 0x30u;

    /* packing signal GasCylinderPresMeasSt_523690*/
    rData = (unsigned int)(GasCylinderPresMeasSt_523690 - (0));
    buff[1u] |= ((unsigned char)(rData << 2u)) & 0x0Cu;

    /* packing signal BufTankPresMeasSt_523689*/
    rData = (unsigned int)(BufTankPresMeasSt_523689 - (0));
    buff[1u] |= ((unsigned char)(rData << 0u)) & 0x03u;

    /* packing signal Reserve1*/
    rData = (unsigned int)(Reserve1 - (0));
    buff[0u]  = ((unsigned char)(rData << 6u)) & 0xC0u;

    /* packing signal LiquidLevelSenAcSt_523687*/
    rData = (unsigned int)(LiquidLevelSenAcSt_523687 - (0));
    buff[0u] |= ((unsigned char)(rData << 4u)) & 0x30u;

    /* packing signal GasCylinderPresSenAcSt_523686*/
    rData = (unsigned int)(GasCylinderPresSenAcSt_523686 - (0));
    buff[0u] |= ((unsigned char)(rData << 2u)) & 0x0Cu;

    /* packing signal BufTankPresSenAcSt_523685*/
    rData = (unsigned int)(BufTankPresSenAcSt_523685 - (0));
    buff[0u] |= ((unsigned char)(rData << 0u)) & 0x03u;
}
