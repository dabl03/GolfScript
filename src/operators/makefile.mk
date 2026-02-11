ADD_SRC=$(SRC_OPERATOR)/add
SET_FLOAT_SRC=$(SRC_OPERATOR)/set_float

# Se agrega de manera manual para compatibilidad con Windows.

$(BIN_OPT)/%.o:	$(ADD_SRC)/%.c $(ALL_DEPENDENCIES)
	@echo Compilando el archivo objeto de $<...
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2>$(SET_LOG_OPT) $(SHOW_LOG_OPT)
	@$(DELETE_V_LOG) $(SET_LOG_OPT)


$(BIN_OPT)/%.o:	$(SET_FLOAT_SRC)/%.c $(ALL_DEPENDENCIES)
	@echo Compilando el archivo objeto de $<...
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2>$(SET_LOG_OPT) $(SHOW_LOG_OPT)
	@$(DELETE_V_LOG) $(SET_LOG_OPT)