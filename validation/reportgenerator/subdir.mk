################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
./LaTeXprocessor.cxx \
./ReportGenerator.cxx \
./Tokenprocessor.cxx 

OBJS += \
./LaTeXprocessor.o \
./ReportGenerator.o \
./Tokenprocessor.o 

CXX_DEPS += \
./LaTeXprocessor.d \
./ReportGenerator.d \
./Tokenprocessor.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Wall -c -g -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


