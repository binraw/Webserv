# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: florian <florian@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 09:47:22 by fberthou          #+#    #+#              #
#    Updated: 2025/01/18 17:41:57 by florian          ###   ########.fr        #
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
INCLUDES	= ./includes
HDR_PATH	= ./sources
SRC_PATH	= ./sources

#---------------------# ==== TEMPORARY FILES DIRECTORIES ==== #----------------#
TEMP_PATH	= .temp
OBJ_PATH	= $(TEMP_PATH)/obj
DEP_PATH	= $(TEMP_PATH)/dep

#-----------------------# ==== COMPILATION FLAGS ==== #------------------------#
MODE		?= release

ifeq ($(MODE), debug)
	COMPFLAGS = -g3 -Wall -Wextra -Wshadow -Wpedantic -std=c++98 -DTEST
else
	COMPFLAGS = -Wall -Wextra -Werror -Wshadow -Wpedantic -std=c++98
endif

DEPFLAGS	= -MM -MT $@ $< -MF $(DEP_PATH)/$*.d


#------------------------# ==== FIND FILES ==== #-----------------------------#
SRC	= $(shell find $(SRC_PATH) -name "*.cpp")
HDR	= $(shell find $(SRC_PATH) -name "*.hpp")

#------------------------# ==== TEMPORARY FILES ==== #-------------------------#
OBJ	= $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRC))
DEP	= $(patsubst $(SRC_PATH)/%.cpp, $(DEP_PATH)/%.d, $(SRC))

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
	$(MAKE) re MODE=debug
	@echo
	$(VALGRIND) --leak-check=full --track-fds=yes ./$(NAME)
#-------------------# ==== LINKING & BUILDING PROGRAM ==== #-------------------#
$(NAME)	: $(OBJ)
	@echo "$(GREEN)-- compilation completed --$(RESET)"
	$(CXX) $^ -o $@
	@echo "$(GREEN)-- linking & building completed --$(RESET)"

#--------------------# ==== COMPILATION OBJ - DEPS ==== #----------------------#
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp Makefile 
	@$(MKD) $(dir $@) $(DEP_PATH)
	$(CXX) $(COMPFLAGS) -I$(HDR_PATH) -I$(INCLUDES) -c $< -o $@
	@$(CXX) $(DEPFLAGS) -I$(HDR_PATH) -I$(INCLUDES)

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
