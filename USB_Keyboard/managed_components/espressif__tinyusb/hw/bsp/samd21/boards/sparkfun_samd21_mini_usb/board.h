/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020, Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

/* metadata:
   name: SparkFun SAMD21 Mini
   url: https://www.sparkfun.com/products/13664
*/

#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
 extern "C" {
#endif

// LED
#define LED_PIN               /*PA*/17 /*(D13)*/
#define LED_STATE_ON          1

// Button
#define BUTTON_PIN            /*PA*/14 /*(D2)*/
#define BUTTON_STATE_ACTIVE   0

// UART
#define UART_SERCOM           0
#define UART_RX_PIN           /*PA*/11 /*(D0)*/
#define UART_TX_PIN           /*PA*/10 /*(D1)*/

#ifdef __cplusplus
 }
#endif

#endif /* BOARD_H_ */
