#ifndef CS_HTML_HEADER
#define CS_HTML_HEADER


#include "cs/config.hpp"
#include "cs/containers/list.hpp"
#include "cs/core/issue.hpp"

#include <fstream>
#include <map>
#include <vector>


#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H T M L -------------------------------------------------------------

	// generate modern website to display c/c++ compilation errors
	// issue class contains file path, line, column, message, and code snippet (code is actually not implemented)




	// generate json file to display c/c++ compilation errors

	// exemple:

	/*
		const errors = {

		{
		"files": [
			{
				"fileName": "./inc/meta/type_at.hpp",
				"errors": [
					{
						"lineNumber": 27,
						"columnNumber": 4,
						"message": "error: static_assert failed due to requirement '3UL < sizeof...(A)' 'TYPE_AT: INDEX OUT OF RANGE!'"
					},
					{
						"lineNumber": 65,
						"columnNumber": 2,
						"message": "note: in instantiation of template class 'xns::impl::type_at<3, X<char, {\"C\"}, {\"char\"}>, X<bool, {\"B\"}, {\"bool\"}>, X<double, {\"D\"}, {\"double\"}>>' requested here"
					}
				]
			},
			{
				"fileName": "./inc/indexed_element.hpp",
				"errors": [
					{
						"lineNumber": 50,
						"columnNumber": 22,
						"message": "note: in instantiation of template type alias 'type_at' requested here"
					}
				]
			}
			// Autres fichiers et erreurs...
		]
	}
	*/


	/* to json string */
	inline auto to_json(const std::string& str) -> std::string {

		// this fonction transforms a string into a json string
		// for example: "hello" -> "\"hello\""
		//              "hello\nworld" -> "\"hello\\nworld\""

		std::string result;

		result.reserve(str.size() + 2);

		result.push_back('\"');

		for (const char c : str) {

			switch (c) {

				case '\\': result += "\\\\"; break;
				case '\"': result += "\\\""; break;
				case '\b': result += "\\b";  break;
				case '\f': result += "\\f";  break;
				case '\n': result += "\\n";  break;
				case '\r': result += "\\r";  break;
				case '\t': result += "\\t";  break;
				default:
					if (c >= '\x00' && c <= '\x1f') {
						result += "\\u" + std::to_string(static_cast<int>(c));
					}
					else { result.push_back(c); }
			}

		}

		result.push_back('\"');

		return result;
	}



	inline auto generate_js(cs::list<cs::issue> ___lst) -> void {

		// Group errors by file
		std::map<std::string, cs::list<cs::issue>> ___map;

		for (const auto& error : ___lst) {
			std::string path{error.path().data(), error.path().size()};
			___map[path].push_back(error);
		}


		//std::ofstream file("issues.js");
		std::ofstream file("issues.json");

		//file << "const errors = ";

		file << "{\n";
		file << "\"files\": [\n";


		for (const auto& f : ___map) {
			file << "{\n";
			file << "\"filename\": " << to_json(f.first) << ",\n";
			file << "\"errors\": [\n";

			for (const auto& error : f.second) {
				file << "{\n";
				file << "\"line\": " << error.line() << ",\n";
				file << "\"column\": " << error.column() << ",\n";

				std::string message{error.message().data(), error.message().size()};

				file << "\"message\": " << to_json(message) << "\n";
				file << "},\n";
			}

			file << "]\n";
			file << "},\n";
		}

		file << "]\n";
		file << "}\n";

		file.close();



	}





	class html final {


		private:

			// -- private types -------------------------------------------------

			/* self type */
			using ___self = cs::html;

	};

} // namespace cs

#endif // ___cs_requirements

#endif // CS_HTML_HEADER


	inline void generate_html(cs::list<cs::issue>& ___errs) {

		return;
		//std::vector<cs::issue> errors;
		//
		//for (cs::issue& error : ___errs) {
		//	errors.push_back(std::move(error));
		//}


		std::ofstream file("err.html");
		file << "<!DOCTYPE html>\n<html lang='en'><head><meta charset='UTF-8'><title>Compilation Errors</title>";
		file << "<style>body{font-family: Arial, sans-serif;} .error{margin-bottom: 20px;} .menu{background: #f0f0f0; padding: 10px;} select{padding: 5px; margin-right: 10px;} pre{background-color: #eeeeee; padding: 10px;}</style>";
		file << "</head><body><h1>Compilation Errors</h1>";

		// Group errors by file
		std::map<std::string, cs::list<cs::issue>> ___map;

		for (const auto& error : ___errs) {
			std::string path{error.path().data(), error.path().size()};
			___map[path].push_back(error);
		}

		// Dropdown menu for file selection
		file << "<div class='menu'><label for='fileSelect'>Choose a file:</label><select id='fileSelect' onchange='showErrors(this.value);'>";
		file << "<option value=''>Select a file</option>";

		for (const auto& filename : ___map) {
			file << "<option value='" << filename.first << "'>" << filename.first << "</option>";
		}
		file << "</select></div>";

		// Display errors
		for (const auto& f : ___map) {
			file << "<div id='" << f.first << "' class='errors' style='display:none;'>";
			for (const auto& error : f.second) {
				file << "<div class='error'>";
				file << "<h2>Error at line " << error.line() << ", column " << error.column() << "</h2>";
				std::string message{error.message().data(), error.message().size()};
				file << "<p>" << message << "</p>";
				file << "</div>";
			}
			file << "</div>";
		}

		// JavaScript to handle dropdown
		file << "<script>function showErrors(fileName) { var errors = document.querySelectorAll('.errors'); errors.forEach(e => e.style.display = 'none'); if (fileName) { document.getElementById(fileName).style.display = 'block'; }}</script>";
		file << "</body></html>";
		file.close();
		/*
		*/
	}


