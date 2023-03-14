/*
 * DEBUG_Prints.h
 *
 *  Created on: 14 Mar 2023
 *      Author: bbels
 */

#ifndef DEBUG_PRINTS_H_
#define DEBUG_PRINTS_H_

#define DEBUG

#ifdef DEBUG
    #define DBprintf(...) printf("DEBUG: " __VA_ARGS__)
#else
    #define DBprintf(...) do {} while (0)
#endif

#endif /* DEBUG_PRINTS_H_ */
