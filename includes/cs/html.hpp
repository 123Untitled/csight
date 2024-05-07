#ifndef CS_HTML_HEADER
#define CS_HTML_HEADER


#include "cs/config.hpp"
#include "cs/core/issue.hpp"
#include "cs/string.hpp"
#include "cs/map.hpp"
#include "cs/system/write.hpp"
#include "cs/system/open.hpp"

#include <fstream>


#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {






	// -- H T M L -------------------------------------------------------------

	// generate modern website to display c/c++ compilation errors
	// issue class contains file path, line, column, message, and code snippet (code is actually not implemented)


	/* to json string */
	inline auto to_json_string(const cs::string& str) -> cs::string {

		// this function escapes special characters in a string to make it a valid JSON string

		cs::string json;

		json.reserve(str.size() + (str.size() / 2U));

		json.push_back('"');

		for (cs::string::size_type i = 0U; i < str.size(); ++i) {

			switch (str[i]) {

				// backslash
				case '\\':
					json.append("\\\\");
					break;

				// double quote
				case '"':
					json.append("\\\"");
					break;

				default:

					// check control character
					if (cs::is_ctrl(str[i])) {

						// convert to unicode
						char buff[] {
							'\\', 'u', '0', '0', '0', '0', '\0'
						};

						// convert to hex
						auto num = static_cast<unsigned char>(str[i]);

						// convert to hex

						unsigned j =  6U;

						do {
							buff[--j] = "0123456789abcdef"[num & 0x0f];
							num >>= 4;
						} while (num);

						// call array append
						json.append(buff);
					}
					else
						json.push_back(str[i]);
			}

		}

		json.push_back('"');

		return json;
	}


	/* to html string */
	inline auto to_html_string(const cs::string& str) -> cs::string {

		// this function escapes special characters in a string to make it a valid HTML string

		cs::string html;

		html.reserve(str.size() + (str.size() / 2U));

		for (cs::string::size_type i = 0U; i < str.size(); ++i) {

			switch (str[i]) {

				// ampersand
				case '&':
					html.append("&amp;");
					break;

				// less than
				case '<':
					html.append("&lt;");
					break;

				// greater than
				case '>':
					html.append("&gt;");
					break;

				// double quote
				case '"':
					html.append("&quot;");
					break;

				// single quote
				case '\'':
					html.append("&apos;");
					break;

				// new line
				case ' ':
					html.append("&nbsp;");
					break;

				default:
					html.push_back(str[i]);
			}

		}

		return html;
	}



	inline auto generate_json(const cs::core::map& ___map) -> cs::string {


		cs::string json;
		json.reserve(1024);

		//json.append("{\n\t\"files\": [\n");
		// non-formated version
		json.append("{\"files\":[");

		___map.for_each([](const cs::core::entry& e, cs::string& json) {

			//json.append("\t\t{\n\t\t\t\"file\": ",
			//			to_json_string(e.file()), ",\n");
			// non-formated version
			json.append("{\"file\":",
						to_json_string(e.file()), ",");

			const auto& i = e.issues();

			//json.append("\t\t\t\"issues\": [\n");
			// non-formated version
			json.append("\"issues\":[");

			i.for_each([](const cs::core::issue& i, cs::string& json) {

				const auto line = std::to_string(i.line());
				const auto column = std::to_string(i.column());

				//json.append("\t\t\t\t{\n\t\t\t\t\t\"line\": ",
				//			cs::string_view{line.data(), line.size()},
				//			",\n\t\t\t\t\t\"column\": ",
				//			cs::string_view{column.data(), column.size()},
				//			",\n\t\t\t\t\t\"message\": ",
				//			to_json_string(i.message()),
				//			"\n\t\t\t\t},\n");

				// non-formated version
				json.append("{\"line\":",
							cs::string_view{line.data(), line.size()},
							",\"column\":",
							cs::string_view{column.data(), column.size()},
							",\"message\":",
							to_json_string(i.message()),
							"},");

			}, json);

			//json.pop_back(2U);
			//json.append("\n\t\t\t]\n\t\t},\n");
			// non-formated version
			json.pop_back(1U);
			json.append("]},");

		}, json);


		//json.pop_back(2U);
		//json.append("\n\t]\n}\n");
		// non-formated version
		json.pop_back(1U);
		json.append("]}");


		const auto dsc = cs::open("site/issues.json", O_WRONLY | O_CREAT | O_TRUNC, 0644);

		cs::write(dsc, json);

		return json;
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
