/*
 * AppConfig.h
 *
 *  Created on: Jul 9, 2018
 *      Author: MSI
 */

#ifndef APPLICATION_APPCONFIG_H_
#define APPLICATION_APPCONFIG_H_

#define VER_1               1
#define VER_2               2
#define HARDWARE_VERSION    VER_1


#define APP_STAND_ALONE     1
#define APP_RTOS            2

#define APP_PLATFORM        APP_STAND_ALONE

#define DEBUG
//#define CONSOLE

#define FTBCLK   40000000   //Hz. Sử dụng với chip TMS320F28020
//#define FTBCLK   60000000   //Hz. Sử dụng với chip TMS320F28026/28027

#define TIMER0_PERIOD       1000        // interrupt in uSecond
#define TIMER1_PERIOD       40  //1000        // interrupt in uSecond
#define TIMER2_PERIOD       1000        // interrupt in uSecond

// Định nghĩa tham số để build chương trình.
#define Build_Boost_Only        1     // Chỉ cho chạy tầng boost
#define Build_Inverter_Fix      2     // Chỉ cho chạy tầng inverter, hệ số điều biên cố định
#define Build_Inverter_All      3     // Cho chạy chức năng boost - inverter hoàn thiện, không quan tâm đến tín hiệu điện áp AC đầu vào.
#define Build_Ups_All           4     // Cho chạy chức năng hoàn thiện, bao gồm inveter, ATS

#define Build_Option            Build_Inverter_Fix

// Định nghĩa tham số acquy.
//Khi sử dụng trong chương trình chỉ chọn một trong các cấu hình này.

//#define Batt_Volt_Sys      12   // Vdc. Định nghĩa điện áp acquy mặc định
#define Batt_Volt_Sys      24   // Vdc. Định nghĩa điện áp acquy mặc định

// Nếu điện áp acquy được chọn là 12 thì gán tham số acquy như sau:
// chỉ khi điện áp acquy trong dải giữa under và over mới cho mạch công suất hoạt động.
#if Batt_Volt_Sys == 12
#define Batt_Under_Volt    10.5 // Vdc. Định nghĩa điện áp acquy thấp
#define Batt_Over_Volt     14.5 // Vdc. Định nghĩa điện áp acquy cao.
#define Batt_Hyst_Volt     0.5  // Vdc. Định nghĩa mắt trễ cho bảo vệ điện áp hoặc cao
#elif Batt_Volt_Sys == 24

// Nếu điện áp acquy được chọn là 24 thì gán tham số acquy như sau:
#define Batt_Under_Volt    21.0 // Vdc. Định nghĩa điện áp acquy thấp
#define Batt_Over_Volt     29.0 // Vdc. Định nghĩa điện áp acquy cao.
#define Batt_Hyst_Volt     1.0  // Vdc. Định nghĩa mắt trễ cho bảo vệ điện áp
#define Batt_Volt_Coff     0.1  // Hệ số dùng tính toán giá trị điện áp từ giá trị ADC
#else
#error We must define valid battery parameter 12/24
#endif

// Định nghĩa tham số phần mềm mạch BOOST điện áp.
#define Boost_SW_Freq           (50000)      //hz, tần số chuyển mạch mạch boost tính theo Khz.
#define Boost_SW_Period         ((FTBCLK / Boost_SW_Freq) - 1)
#define Boost_Duty_Coeff        (0.95)       //Duty cycle tối đa của mạch cầu H. CHuyển mạch cứng tương tự Push-pull.
#define Boost_SW_Max_Duty       ((Boost_Duty_Coeff * Boost_SW_Period) / 2)
#define Boost_Start_Percen_F    (10.0)

#define Boost_Ki                0.1 // Hệ số Ki tầng boost
#define Boost_Kp                0.1 // Hệ số Kp tầng boost
#define Boost_Kd                0.1 // Hệ số Kd tầng boost

#define Boost_Method_Lookup     1
#define Boost_Method_Pid        2

#define Boost_Method_Used       Boost_Method_Lookup


// Định nghĩa tham số bảo vệ tầng BOOST điện áp.
#define Boost_Trip_Curr                 2000 // mVdc. Mức điện áp bảo vệ quá dòng cho mạch Boost
#define Boost_Fault_Clear_Time          100  // ms. Dải từ 1-1000ms. Thời gian clear sự kiện quá dòng.
#define Boost_Stage_Restart_Time        10   // Dải từ 0-100. Số lần clear bảo vệ quá dòng, hết số lần này khóa mạch luôn.

#define Boost_Voltage_Adc_Coeff         (0.2142857142857143)  // convert ADC value to volt


// Định nghĩa tham số tầng inverter.
// Tính toán để hỗ trợ tần số cơ bản tối thiểu 100hz, tần số PWM đến 100kHz
// Hệ điều khiển tính toán ra tỷ số điều biên ở mạch inverter cao áp là kiến trúc vòng hở, căn cứ vào điện áp acquy và dòng điện một chiều DC BUS

#define PWM_Freq                        10   // kHz. Dải từ 5-100kHz. Tần số điều chế tính theo Khz
#define PWM_Dead_Time                   500  // ns. Dải từ 100-3000ns. Thời gian dead_time.
#define Base_Freq                       50000   // Hz. Dải từ 45-400Hz. Tần số cơ bản điện áp đầu ra.
#define PWM_Modulation_SVM              0    // Chọn kiểu điều chế SVM. Giá trị là 1 hoặc 0.
#define PWM_Modulation_SinPWM           1    // Chọn kiểu điều chế sin. Giá trị là 1 hoặc 0.
#define Inverter_Dcbus_Under            300  // Vdc. Điện áp DC đầu vào bắt đầu cho chạy inverter.
#define Inverter_Dcbus_Over             350  // Vdc. Điện áp DC đầu vào ngắt inverter, không cho phép chạy lại.

#define Inverter_Output_Freq            50
#define Inverter_Update_Freq            16000


#define Inverter_Step_Mf                0.01
#define Inverter_Step_Angle             0.72

#define Inverter_Start_Mf               0.2
#define Inverter_Max_Mf                 0.82495     //0.9  // Tỷ số điều biên cố định

#define Inverter_Output_Current_Trip    2000 // mV, Điện áp tương ứng với dòng điện bảo vệ quá tải đo tại chân ADC.
#define Inverter_ Fault_Clear_Time      1000 // ms, thời gian clear bảo vệ quá tải.
#define Inverter_Trip_Restart_Time      10   // Dải từ 0-100. Số lần clear bảo vệ quá tải, hết số lần này khóa mạch luôn.
#define Inverter_Soft_Start_Time        1000 // ms. Thời gian khởi động mềm.





// Định nghĩa tham số cho chức năng chuyển mạch điện lưới - inverter
#define AC_Detect_Level   2500  // mV. Điện áp AC_detect cao hơn ngưỡng này được hiểu là sự kiện mất lưới.
#define AC_Delay_Time     1000  // ms. Dải từ 0-60000ms. Thời gian delay từ lúc xác nhận sự kiện mất lưới đến khi khởi động inverter.



#define AC_Detect_Avrg                  100

#define Inverter_Type_Open_Half         0
#define Inverter_Type_Open_Full         1

#define Inverter_Switching_Type         Inverter_Type_Open_Half


/** TEST OPTION **/

//#define TEST_INV_PWM_SETTING



#endif /* APPLICATION_APPCONFIG_H_ */
