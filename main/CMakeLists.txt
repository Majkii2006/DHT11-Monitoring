idf_component_register(SRCS "main.c" "../components/7SEG_MODULE/7SEG_MODULE.c" 
	INCLUDE_DIRS "." "../components/7SEG_MODULE/include"
	REQUIRES esp_driver_gpio)
