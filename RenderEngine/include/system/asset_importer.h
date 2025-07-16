#pragma once
#include "dependencies/sqlite3.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "other utils/IDGEN.h"


struct asset_details {

	std::string path;
	std::string id;
    std::string type;

};


class asset_importer {
public:
    std::vector<asset_details> m_all_assets;

    asset_importer(std::string path = "assets.db") {
        initialize_db(path);
    }

    ~asset_importer() {
        sqlite3_close(db);
    }

    void edit_asset_path(const std::string& id, const std::string& new_path) {

        std::string query = "SELECT id FROM assets WHERE id ='" + id + "';";
        bool asset_exists = false;

        rc = sqlite3_exec(db, query.c_str(), [](void* exists, int count, char** data, char** columns) -> int {
            (void)data;
            (void)columns;
            (void)count;
            *static_cast<bool*>(exists) = true;
            return 0;
            }, &asset_exists, &zErrMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error during check: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            return;
        }

        if (!asset_exists) {
            std::cerr << "Asset with ID '" << id << "' does not exist." << std::endl;
            return;
        }

        std::string sql = "UPDATE assets SET path = '" + new_path + "' WHERE id = '" + id + "';";
        rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error during update: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
            std::cout << "Asset path updated successfully for ID: " << id << std::endl;
        }
    }


    void remove_from_database(std::string id) {
        std::string sql = "DELETE FROM assets WHERE id ='" + id + "';";
        rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
    }

    void add_to_database(const asset_details& asset) {
        
        if (!std::filesystem::exists(asset.path)) {
            throw std::invalid_argument("The asset path does not exist: " + asset.path);
        }

        const auto all_assets = retrieve_all_assets();
        for (const auto& a : all_assets) {
            if (std::filesystem::equivalent(std::filesystem::path(asset.path), std::filesystem::path(a.path))) {
                return;
            }
        }

        insert_asset(asset);
    }

    void insert_asset(const asset_details& asset) {
        std::string sql = "INSERT INTO assets (id, path, type) VALUES ('" + lowlevelsys::generate_unique_id() + "', '" + asset.path + "', '" + asset.type + "');";
        rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
    }

    std::vector<asset_details> retrieve_all_assets() {
        rearrange_assets();
        m_all_assets.clear();
        std::string sql = "SELECT id, path, type FROM assets;";
        rc = sqlite3_exec(db, sql.c_str(), callback, this, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        return m_all_assets;
    }

    void rearrange_assets() {
        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, 0, &zErrMsg);

        // Create a temporary table to store the reordered assets
        std::string create_temp_table =
            "CREATE TEMP TABLE temp_assets AS "
            "SELECT * FROM assets ORDER BY "
            "CASE "
            "WHEN type = 'texture' THEN 1 "
            "WHEN type = 'material' THEN 2 "
            "ELSE 3 END;";
        rc = sqlite3_exec(db, create_temp_table.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error while creating temp table: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_exec(db, "ROLLBACK;", nullptr, 0, &zErrMsg);
            return;
        }

        // Clear the original table
        std::string clear_table = "DELETE FROM assets;";
        rc = sqlite3_exec(db, clear_table.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error while clearing original table: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_exec(db, "ROLLBACK;", nullptr, 0, &zErrMsg);
            return;
        }

        // Reinsert assets from the temporary table into the original table
        std::string reinsert_assets = "INSERT INTO assets SELECT * FROM temp_assets;";
        rc = sqlite3_exec(db, reinsert_assets.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error while reinserting from temp table: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_exec(db, "ROLLBACK;", nullptr, 0, &zErrMsg);
            return;
        }

        // Drop the temporary table
        std::string drop_temp_table = "DROP TABLE temp_assets;";
        rc = sqlite3_exec(db, drop_temp_table.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error while dropping temp table: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_exec(db, "ROLLBACK;", nullptr, 0, &zErrMsg);
            return;
        }

        sqlite3_exec(db, "COMMIT;", nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error during commit: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
            std::cout << "Assets successfully rearranged." << std::endl;
        }
    }



private:
    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;

    static int callback(void* data, int argc, char** argv, char** azColName) {
        auto* importer = static_cast<asset_importer*>(data);
        asset_details asset;
        for (int i = 0; i < argc; i++) {
            std::string colName = azColName[i];
            if (colName == "id") {
                asset.id = argv[i] ? argv[i] : "";
            }
            else if (colName == "path") {
                asset.path = argv[i] ? argv[i] : "";
            }
            else if (colName == "type") {
                asset.type = argv[i] ? argv[i] : "";
            }
        }
        importer->m_all_assets.push_back(asset);
        return 0;
    }

    void initialize_db(std::string path) {
        rc = sqlite3_open(path.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        else {
    
        }

        const char* sql = "CREATE TABLE IF NOT EXISTS assets ("
            "id TEXT PRIMARY KEY NOT NULL, "
            "path TEXT NOT NULL, "
            "type TEXT NOT NULL);";
        rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
         
        }
    }
};
