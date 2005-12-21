struct {
  const char *name, *value;
} config_defaults[] = {
  { "bindir", BINDIR},
  { "sysconfdir", SYSCONFDIR},
  { "version", EYEDB_VERSION},
  { "numversion", EYEDB_NUMVERSION},
  { 0, 0}
};
