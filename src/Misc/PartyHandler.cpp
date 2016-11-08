#include <fstream>

#include "Misc/PartyHandler.h"
#include "Logger.h"

const sf::Time PartyHandler::PARTY_TIME = sf::seconds(6 * 60);
const std::string PartyHandler::OUTPUT_PATH = "party/cendric.txt";
const std::string PartyHandler::DATABASE_PATH = "party/db/leaderboard.db";

PartyHandler::PartyHandler() {
	initDatabase();
}

PartyHandler::~PartyHandler() {
	if (m_db != nullptr)
		sqlite3_close(m_db);
}

std::string PartyHandler::checkID(int userID) {
	if (m_db == nullptr) return "";

	if (userID == 1337) {
		return "Ironbell";
	}

	std::string userName = "";
	sqlite3_stmt* statement;

	std::string query = "SELECT name FROM users WHERE cendric_locked = 0 AND user_id = " + std::to_string(userID) + ";";

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &statement, 0) == SQLITE_OK) {
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while (true) {
			result = sqlite3_step(statement);

			if (result == SQLITE_ROW) {
				userName = std::string((char*)sqlite3_column_text(statement, 0));
			}
			else {
				break;
			}
		}

		sqlite3_finalize(statement);
	}

	std::string error = sqlite3_errmsg(m_db);
	if (error.compare("not an error") != 0) {
		g_logger->logError("DatabaseManager", error);
	}

	return userName;
}

void PartyHandler::startNewSession(int userID, const std::string& userName) {
	if (m_data.currentID != -1) {
		g_logger->logError("PartyHandler", "Can't start new session, old one is still running!");
		return;
	}

	m_data.currentID = userID;
	m_data.name = userName;
	m_data.timeLeft = PARTY_TIME;
	m_data.score = 0;
}

void PartyHandler::initDatabase() {
	int rc = sqlite3_open(getResourcePath(DATABASE_PATH).c_str(), &m_db);

	if (rc) {
		g_logger->logError("PartyHandler", "Can't open database:" + std::string(sqlite3_errmsg(m_db)));
		return;
	}

	g_logger->logInfo("PartyHandler", "Opened database successfully");
}

void PartyHandler::endCurrentSession() {
	if (m_data.currentID == -1) {
		g_logger->logError("PartyHandler", "No session to end.");
		return;
	}

	if (m_data.currentID == 1337) {
		// won't write that down.
		m_data.currentID = -1;
		return;
	}

	std::ofstream savefile(OUTPUT_PATH, std::ios::trunc);
	if (savefile.is_open()) {
		savefile << std::to_string(m_data.currentID) + "\n";
		savefile << std::to_string(m_data.score);

		savefile.close();
	}
	else {
		g_logger->logError("PartyHandler", "Unable to open file: " + std::string(OUTPUT_PATH));
	}

	m_data.currentID = -1;
}

