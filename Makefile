# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rtruvelo <rtruvelo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 09:47:22 by fberthou          #+#    #+#              #
#    Updated: 2025/02/05 11:21:38 by rtruvelo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==============================================================================#
#                              SETTING VARIABLES                               #
#==============================================================================#

RESET		= \033[0m
GREEN		= \033[32m
YELLOW		= \033[33m

#------------------------# ==== EXECUTABLE NAME ==== #-------------------------#
NAME		= webserv

#------------------------# ==== DEFAULT COMPILER ==== #------------------------#
CXX			= c++

#---------------------------# ==== SHELL CMDS ==== #---------------------------#
RM          = rm -rf
MKD			= mkdir -p
VALGRIND	= valgrind --leak-check=full 

#-----------------------# ==== SOURCE CODE DIRECTORIES ==== #------------------#
HDR_PATH	= ./headers
SRC_PATH	= ./sources

#---------------------# ==== TEMPORARY FILES DIRECTORIES ==== #----------------#
TEMP_PATH	= .temp
OBJ_PATH	= $(TEMP_PATH)/obj
DEP_PATH	= $(TEMP_PATH)/dep

#-----------------------# ==== COMPILATION FLAGS ==== #------------------------#
MODE		?= release

ifeq ($(MODE), debug)
	COMPFLAGS = -g3 -Wall -Wextra -Wshadow -Wpedantic -std=c++98
else
	COMPFLAGS = -Wall -Wextra -Werror -Wshadow -Wpedantic -std=c++98
endif

DEPFLAGS	= -MM -MT $@ $< -MF $(DEP_PATH)/$*.d

#------------------------# ==== MANDATORY FILES ==== #-------------------------#
SRC	= main.cpp  UtilParsing.cpp ConfigParser.cpp HttpConfig.cpp LocationConfig.cpp ServerConfig.cpp
# HEADERS_INC = Server.hpp

#------------------------# ==== TEMPORARY FILES ==== #-------------------------#
OBJ	= $(SRC:%.cpp=$(OBJ_PATH)/%.o)
DEP	= $(SRC:%.cpp=$(DEP_PATH)/%.d)

#==============================================================================#
#                            COMPILATION MANDATORY                             #
#==============================================================================#
default: all

all: $(NAME)
	@echo
	@echo "$(GREEN)***************** $(NAME) is READY *****************$(RESET)"
	@echo

debug:
	$(MAKE) MODE=debug
	@echo
	@echo "$(GREEN)************* $(NAME) is in debug mode *************$(RESET)"
	@echo

test:
	clear
	$(MAKE) re MODE=debug
	$(VALGRIND) ./$(NAME)
#-------------------# ==== LINKING & BUILDING PROGRAM ==== #-------------------#
$(NAME)	: $(OBJ)
	@echo "$(GREEN)-- compilation completed --$(RESET)"
	$(CXX) $^ -o $@
	@echo "$(GREEN)-- linking & building completed --$(RESET)"

#--------------------# ==== COMPILATION OBJ - DEPS ==== #----------------------#
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp Makefile 
	@$(MKD) $(dir $@) $(DEP_PATH)
	$(CXX) $(COMPFLAGS) -I$(HDR_PATH) -c $< -o $@
	@$(CXX) $(DEPFLAGS) -I$(HDR_PATH)

-include $(DEP)

#------------------------------------------------------------------------------#
############################# RECOMPILATION CALL ###############################
#------------------------------------------------------------------------------#
re:
	@$(MAKE) fclean
	$(MAKE) all

re_debug:
	@$(MAKE) fclean
	$(MAKE) MODE=debug

#------------------------------------------------------------------------------#
############################### CLEANING RULES #################################
#------------------------------------------------------------------------------#
clean:
	@$(RM) $(TEMP_PATH)
	@echo "$(YELLOW)-- removed temporary files --$(RESET)"

fclean:
	@$(RM) $(TEMP_PATH) $(NAME)
	@echo "$(YELLOW)-- removed $(NAME) & temporary files --$(RESET)"


.PHONY: default all debug test re re_debug clean fclean