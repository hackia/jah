#include "Atom.hpp"
#include "Model.hpp"
#include <blake3.h>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <vector>

using namespace Jah;
using namespace std;

const auto JAH_USERNAME = getenv("JAH_USERNAME");
const auto JAH_EMAIL = getenv("JAH_EMAIL");

Atom::Atom(string type, string summary, string body, string note, string footer,
           string id)
    : type(std::move(type)), summary(std::move(summary)),
      footer(std::move(footer)), body(std::move(body)), issue(std::move(id)),
      note(std::move(note)) {}

string Atom::getHash(const Config &config) const {
  stringstream content_to_hash;
  content_to_hash << "type:" << this->type << "\n"
                  << "summary:" << this->summary << "\n"
                  << "body:" << this->body << "\n"
                  << "note:" << this->note << "\n"
                  << "footer:" << this->footer << "\n"
                  << "issue:" << this->issue << "\n"
                  << "author:" << config.getAuthor() << " <"
                  << config.getEmail() << ">" << "\n"
                  << "timestamp:" << to_string(std::time(nullptr));

  string final_content = content_to_hash.str();
  blake3_hasher hasher;
  blake3_hasher_init(&hasher);
  blake3_hasher_update(&hasher, final_content.c_str(), final_content.length());
  uint8_t output[BLAKE3_OUT_LEN];
  blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);
  stringstream hex_stream;
  hex_stream << hex << setfill('0');
  for (unsigned char i : output) {
    hex_stream << setw(2) << static_cast<int>(i);
  }
  return hex_stream.str();
}

string Atom::getType() const { return type; }
string Atom::getSummary() const { return summary; }
string Atom::getBody() const { return body; }
string Atom::getFooter() const { return footer; }
string Atom::getNote() const { return note; }

int Atom::save(const Config &config) const {
  const Model model(config);
  PGconn *conn = PQconnectdb(model.getDbUrl().c_str());
  if (PQstatus(conn) != CONNECTION_OK) {
    cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
    PQfinish(conn);
    return EXIT_FAILURE;
  }
  const string query = "INSERT INTO commits (type,hash, summary, body, note, "
                       "footer) VALUES ($1, $2, $3, $4, $5,$6)";
  const char *paramValues[6] = {type.c_str(), JAH_USERNAME, summary.c_str(),
                                body.c_str(), note.c_str(), footer.c_str()};
  const int paramLengths[6] = {static_cast<int>(type.length()),
                               static_cast<int>(strlen(JAH_USERNAME)),
                               static_cast<int>(summary.length()),
                               static_cast<int>(body.length()),
                               static_cast<int>(note.length()),
                               static_cast<int>(footer.length())};
  constexpr int paramFormats[6] = {0, 0, 0, 0, 0, 0};

  PGresult *res = PQexecParams(conn, query.c_str(), 6, nullptr, paramValues,
                               paramLengths, paramFormats, 0);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cerr << "Insert failed: " << PQerrorMessage(conn) << endl;
    PQclear(res);
    PQfinish(conn);
    return EXIT_FAILURE;
  }

  PQclear(res);
  PQfinish(conn);
  cout << "commited" << endl;
  return EXIT_SUCCESS;
}
int Atom::log(const int limit, const Config &config) {
  const Model model(config);
  const string connStr = model.getDbUrl();
  PGconn *conn = PQconnectdb(connStr.c_str());
  if (PQstatus(conn) != CONNECTION_OK) {
    cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
    PQfinish(conn);
    return EXIT_FAILURE;
  }
  string query = "SELECT message FROM commits ORDER BY timestamp DESC";
  if (limit > 0) {
    query += " LIMIT " + to_string(limit);
  }

  PGresult *res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    cerr << "Query failed: " << PQerrorMessage(conn) << endl;
    PQclear(res);
    PQfinish(conn);
    return EXIT_FAILURE;
  }

  const int rows = PQntuples(res);
  for (int i = 0; i < rows; i++) {
    cout << PQgetvalue(res, i, 0) << endl;
  }

  PQclear(res);
  PQfinish(conn);
  return EXIT_SUCCESS;
}
int Atom::accepted() { return 0; }
int Atom::refused() { return 1; }
