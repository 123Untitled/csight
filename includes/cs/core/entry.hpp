#ifndef ___CS_CORE_ENTRY_HPP___
#define ___CS_CORE_ENTRY_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/memory/lifecycle.hpp"
#include "cs/string.hpp"
#include "cs/core/issues.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- C O R E  N A M E S P A C E ------------------------------------------

	namespace core {


		// -- E N T R Y ---------------------------------------------------------

		class entry final {


			private:

				// -- private types -----------------------------------

				/* self type */
				using ___self = cs::core::entry;


				// -- friends ---------------------------------------------

				/* map as friend */
				friend class map;

				/* lifecycle as friend */
				friend class cs::lifecycle<___self>;


				// -- private members ---------------------------------

				/* file */
				cs::string _file;

				/* issues */
				cs::core::issues _issues;


				// -- private lifecycle -------------------------------

				/* deleted default constructor */
				entry(void) = delete;

				/* members constructor */
				entry(cs::string&&, cs::core::issue&&) noexcept;

				/* deleted copy constructor */
				entry(const ___self&) = delete;

				/* deleted move constructor */
				entry(___self&&) = delete;

				/* destructor */
				~entry(void) noexcept = default;


				// -- public assignment operators ---------------------

				/* deleted copy assignment operator */
				auto operator=(const ___self&) -> ___self& = delete;

				/* deleted move assignment operator */
				auto operator=(___self&&) -> ___self& = delete;


			public:

				// -- public accessors --------------------------------

				/* const file */
				auto file(void) const noexcept -> const cs::string&;

				/* const issues */
				auto issues(void) const noexcept -> const cs::core::issues&;

		}; // class entry

	} // namespace core

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_CORE_ENTRY_HPP___
