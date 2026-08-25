/* ---------------------------------------------------------------------------
 * TEMPLATE FILE — copy this to `cert.h` and paste in your broker's CA root
 * certificate.
 *
 *   cp cert.example.h cert.h
 *
 * Only required when HIVEMQS or MOSQUITTOS is defined in profile.h. With the
 * default (unencrypted, port 1883) configuration this file is unused.
 *
 * Where to get the certificate:
 *   - HiveMQ Cloud    → ISRG Root X1, from https://letsencrypt.org/certs/
 *   - test.mosquitto  → https://test.mosquitto.org/ssl/mosquitto.org.crt
 *
 * Paste the PEM block verbatim, including the BEGIN/END lines.
 * ------------------------------------------------------------------------- */

static const char* root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
YOUR_BROKER_CA_ROOT_CERTIFICATE_HERE
-----END CERTIFICATE-----
)EOF";
