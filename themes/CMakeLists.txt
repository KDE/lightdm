install(DIRECTORY classic
        DESTINATION ${DATA_INSTALL_DIR}/lightdm-kde-greeter/themes
        PATTERN Messages.sh EXCLUDE
)
install(DIRECTORY userbar
        DESTINATION ${DATA_INSTALL_DIR}/lightdm-kde-greeter/themes
        PATTERN Messages.sh EXCLUDE
)

