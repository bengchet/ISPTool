/**************************************************************************//**
 * @file     pwm.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 15/04/07 2:47p $ 
 * @brief    NM1200_NM1100 PWM driver header file
 *
 * @note
 * Copyright (C) 2015 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/ 
#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NM1200_NM1100_Device_Driver NM1200_NM1100 Device Driver
  @{
*/

/** @addtogroup NM1200_NM1100_PWM_Driver PWM Driver
  @{
*/

/** @addtogroup NM1200_NM1100_PWM_EXPORTED_CONSTANTS PWM Exported Constants
  @{
*/
#define PWM_CHANNEL_NUM                     (6)   /*!< PWM channel number */
#define PWM_CLK_DIV_1                       (4UL) /*!< PWM clock divide by 1 */
#define PWM_CLK_DIV_2                       (0UL) /*!< PWM clock divide by 2 */
#define PWM_CLK_DIV_4                       (1UL) /*!< PWM clock divide by 4 */
#define PWM_CLK_DIV_8                       (2UL) /*!< PWM clock divide by 8 */
#define PWM_CLK_DIV_16                      (3UL) /*!< PWM clock divide by 16 */
#define PWM_EDGE_ALIGNED                    (0UL)                   /*!< PWM working in edge aligned type */
#define PWM_CENTER_ALIGNED                  (PWM_CTL_CNTTYPE_Msk)   /*!< PWM working in center aligned type */
#define PWM_TRIGGER_ADC_CNTR_IS_0           PWM_ADCTCTL0_ZPTRGEN0_Msk   /*!< PWM trigger ADC while counter matches 0 */
#define PWM_TRIGGER_ADC_CNTR_IS_CMR_D       PWM_ADCTCTL0_CDTRGEN0_Msk   /*!< PWM trigger ADC while counter matches CMR during down count */
#define PWM_TRIGGER_ADC_CNTR_IS_CNR         PWM_ADCTCTL0_CPTRGEN0_Msk   /*!< PWM trigger ADC while counter matches CNR */
#define PWM_TRIGGER_ADC_CNTR_IS_CMR_U       PWM_ADCTCTL0_CUTRGEN0_Msk   /*!< PWM trigger ADC while counter matches CMR during up count  */
#define PWM_FB0_EINT0       (PWM_BRKCTL_BRK0EN_Msk)                              /*!< External interrupt 0 as fault brake 0 source */
#define PWM_FB0_ACMP1       (PWM_BRKCTL_BRK1EN_Msk | PWM_BRKCTL_BRK1SEL_Msk)     /*!< Comparator 1 as fault brake 0 source */
#define PWM_FB1_EINT1       (PWM_BRKCTL_BRK1EN_Msk)                              /*!< External interrupt 1 as fault brake 1 source */
#define PWM_FB1_ACMP0       (PWM_BRKCTL_BRK1EN_Msk | PWM_BRKCTL_BRK0SEL_Msk)     /*!< Comparator 0 as fault brake 1 source */
#define PWM_PERIOD_INT_UNDERFLOW            (0)                                  /*!< PWM period interrupt trigger if counter underflow */
#define PWM_PERIOD_INT_MATCH_CNR            (PWM_INTEN_PINTTYPE_Msk)             /*!< PWM period interrupt trigger if counter match CNR */

/*@}*/ /* end of group NM1200_NM1100_PWM_EXPORTED_CONSTANTS */


/** @addtogroup NM1200_NM1100_PWM_EXPORTED_FUNCTIONS PWM Exported Functions
  @{
*/

/**
 * @brief This macro enable complementary mode
 * @param[in] pwm The base address of PWM module
 * @return None
 * \hideinitializer
 */
#define PWM_ENABLE_COMPLEMENTARY_MODE(pwm) (PWM->CTL = (PWM->CTL & ~PWM_CTL_MODE_Msk) |(1UL << PWM_CTL_MODE_Pos))

/**
 * @brief This macro disable complementary mode, and enable independent mode.
 * @param[in] pwm The base address of PWM module
 * @return None
 * \hideinitializer
 */
#define PWM_DISABLE_COMPLEMENTARY_MODE(pwm) (PWM->CTL &= ~PWM_CTL_MODE_Msk)

/**
 * @brief This macro enable group mode
 * @param[in] pwm The base address of PWM module
 * @return None
 * \hideinitializer
 */
#define PWM_ENABLE_GROUP_MODE(pwm) (PWM->CTL |= PWM_CTL_GROUPEN_Msk)

/**
 * @brief This macro disable group mode
 * @param[in] pwm The base address of PWM module
 * @return None
 * \hideinitializer
 */
#define PWM_DISABLE_GROUP_MODE(pwm) (PWM->CTL &= ~PWM_CTL_GROUPEN_Msk)

/**
 * @brief This macro enable synchronous mode
 * @param[in] pwm The base address of PWM module
 * @return None
 * \hideinitializer
 */
#define PWM_ENABLE_SYNC_MODE(pwm) (PWM->CTL = (PWM->CTL & ~PWM_CTL_MODE_Msk) |(2UL << PWM_CTL_MODE_Pos))
 
/**
 * @brief This macro disable synchronous mode, and enable independent mode.
 * @param[in] pwm The base address of PWM module
 * @return None
 * \hideinitializer
 */
#define PWM_DISABLE_SYNC_MODE(pwm) (PWM->CTL &= ~PWM_CTL_MODE_Msk)

/**
 * @brief This macro enable output inverter of specified channel(s)
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel
 *                           Bit 0 represents channel 0, bit 1 represents channel 1...
 * @return None
 * \hideinitializer
 */
#define PWM_ENABLE_OUTPUT_INVERTER(pwm, u32ChannelMask) \
    do{ \
        int i;\
        for(i = 0; i < 6; i++) { \
            if((u32ChannelMask) & (1 << i)) \
                PWM->CTL |= (PWM_CTL_PINV0_Msk << (PWM_CTL_PINV0_Pos + (i * 4))); \
        } \
    }while(0)

/**
 * @brief This macro set the prescaler of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Prescaler Clock prescaler of specified channel. Valid values are between 1 ~ 0xFF
 * @return None
 * @note Every even channel N, and channel (N + 1) share a prescaler. So if channel 0 prescaler changed, 
 *       channel 1 will also be affected.
 * \hideinitializer
 */
#define PWM_SET_PRESCALER(pwm, u32ChannelNum, u32Prescaler) \
    (PWM->CLKPSC = (PWM->CLKPSC & ~(PWM_CLKPSC_CLKPSC01_Msk << (((u32ChannelNum) >> 1) * 8))) | ((u32Prescaler) << (((u32ChannelNum) >> 1) * 8))) 

/**
 * @brief This macro set the divider of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Divider Clock divider of specified channel. Valid values are
 *              - \ref PWM_CLK_DIV_1
 *              - \ref PWM_CLK_DIV_2
 *              - \ref PWM_CLK_DIV_4
 *              - \ref PWM_CLK_DIV_8
 *              - \ref PWM_CLK_DIV_16 
 * @return None
 * \hideinitializer
 */
#define PWM_SET_DIVIDER(pwm, u32ChannelNum, u32Divider) \
    (PWM->CLKDIV = (PWM->CLKDIV & ~(PWM_CLKDIV_CLKDIV0_Msk << ((u32ChannelNum) * 4))) | ((u32Divider) << ((u32ChannelNum) * 4)))

/**
 * @brief This macro set the duty of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5 
 * @param[in] u32CMR Duty of specified channel. Valid values are between 0~0xFFFF
 * @return None
 * @note This new setting will take effect on next PWM period
 * \hideinitializer
 */
#define PWM_SET_CMR(pwm, u32ChannelNum, u32CMR) (PWM->CMPDAT[u32ChannelNum] = (u32CMR))

/**
 * @brief This macro set the period of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5 
 * @param[in] u32CNR Period of specified channel. Valid values are between 0~0xFFFF
 * @return None
 * @note This new setting will take effect on next PWM period
 * @note PWM counter will stop if period length set to 0
 * \hideinitializer
 */
#define PWM_SET_CNR(pwm, u32ChannelNum, u32CNR)  (PWM->PERIOD[u32ChannelNum] = (u32CNR))

/**
 * @brief This macro set the PWM aligned type
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask This parameter is not used
 * @param[in] u32AlignedType PWM aligned type, valid values are:
 *                  - \ref PWM_EDGE_ALIGNED
 *                  - \ref PWM_CENTER_ALIGNED
 * @return None
 * \hideinitializer
 */
#define PWM_SET_ALIGNED_TYPE(pwm, u32ChannelMask, u32AlignedType) \
    (PWM->CTL = (PWM->CTL & ~PWM_CTL_CNTTYPE_Msk) | (u32AlignedType))


uint32_t PWM_ConfigOutputChannel(PWM_T *pwm,
                                  uint32_t u32ChannelNum, 
                                  uint32_t u32Frequency, 
                                  uint32_t u32DutyCycle);
void PWM_Start(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_Stop(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_ForceStop(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_EnableADCTrigger(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Condition);
void PWM_DisableADCTrigger(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_ClearADCTriggerFlag(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Condition);
uint32_t PWM_GetADCTriggerFlag(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_EnableFaultBrake(PWM_T *pwm, 
                           uint32_t u32ChannelMask, 
                           uint32_t u32LevelMask, 
                           uint32_t u32BrakeSource);
void PWM_ClearFaultBrakeFlag(PWM_T *pwm, uint32_t u32BrakeSource);
void PWM_EnableOutput(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_DisableOutput(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_EnableDeadZone(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Duration);
void PWM_DisableDeadZone(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_EnableDutyInt(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32IntDutyType);
void PWM_DisableDutyInt(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_ClearDutyIntFlag(PWM_T *pwm, uint32_t u32ChannelNum);
uint32_t PWM_GetDutyIntFlag(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_EnableFaultBrakeInt(PWM_T *pwm, uint32_t u32BrakeSource);
void PWM_DisableFaultBrakeInt(PWM_T *pwm, uint32_t u32BrakeSource);
void PWM_ClearFaultBrakeIntFlag(PWM_T *pwm, uint32_t u32BrakeSource);
uint32_t PWM_GetFaultBrakeIntFlag(PWM_T *pwm, uint32_t u32BrakeSource);
void PWM_EnablePeriodInt(PWM_T *pwm, uint32_t u32ChannelNum,  uint32_t u32IntPeriodType);
void PWM_DisablePeriodInt(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_ClearPeriodIntFlag(PWM_T *pwm, uint32_t u32ChannelNum);
uint32_t PWM_GetPeriodIntFlag(PWM_T *pwm, uint32_t u32ChannelNum);



/*@}*/ /* end of group NM1200_NM1100_PWM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NM1200_NM1100_PWM_Driver */

/*@}*/ /* end of group NM1200_NM1100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__PWM_H__

/*** (C) COPYRIGHT 2015 Nuvoton Technology Corp. ***/
