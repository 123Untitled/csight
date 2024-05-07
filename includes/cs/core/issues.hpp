#ifndef ___CS_CORE_ISSUES_HPP___
#define ___CS_CORE_ISSUES_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/core/issue.hpp"
#include "cs/type_traits/declval.hpp"
#include "cs/type_traits/forward.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- C O R E  N A M E S P A C E ------------------------------------------

	namespace core {


		// -- I S S U E S -----------------------------------------------------

		class issues final {


			public:

				// -- public types --------------------------------------------

				/* value type */
				using value_type      = cs::core::issue;

				/* reference type */
				using reference       = value_type&;

				/* const reference type */
				using const_reference = const value_type&;

				/* size type */
				using size_type       = cs::size_t;


			private:

				// -- private types -------------------------------------------

				/* self type */
				using ___self = cs::core::issues;


				// -- private structs -----------------------------------------

				/* node */
				struct ___node final {


					private:

						// -- private types -----------------------------------

						/* self type */
						using ___self = ___self::___node;


					public:

						// -- public members ----------------------------------

						/* issue */
						cs::core::issue _issue;

						/* next */
						___self* _next;


						// -- public lifecycle --------------------------------

						/* deleted default constructor */
						___node(void) = delete;

						/* members constructor */
						___node(cs::core::issue&&) noexcept;

						/* deleted copy constructor */
						___node(const ___self&) = delete;

						/* deleted move constructor */
						___node(___self&&) = delete;

						/* destructor */
						~___node(void) noexcept = default;


						// -- public assignment operators ---------------------

						/* deleted copy assignment operator */
						auto operator=(const ___self&) -> ___self& = delete;

						/* deleted move assignment operator */
						auto operator=(___self&&) -> ___self& = delete;

				}; // struct node


				// -- private members -----------------------------------------

				/* head */
				___node* _head;

				/* tail */
				___node** _tail;

				/* size */
				size_type _size;


			public:

				// -- public lifecycle ----------------------------------------

				/* default constructor */
				issues(void) noexcept;

				/* deleted copy constructor */
				issues(const ___self&) = delete;

				/* move constructor */
				issues(___self&&) noexcept;

				/* destructor */
				~issues(void) noexcept;


				// -- public assignment operators -----------------------------

				/* deleted copy assignment operator */
				auto operator=(const ___self&) -> ___self& = delete;

				/* move assignment operator */
				auto operator=(___self&&) noexcept -> ___self&;


				// -- public accessors ----------------------------------------

				/* size */
				auto size(void) const noexcept -> size_type;

				/* empty */
				auto empty(void) const noexcept -> bool;


				// -- public modifiers ----------------------------------------

				/* push */
				auto push(cs::core::issue&&) -> void;


				// -- public methods ------------------------------------------

				/* for each */
				template <typename ___fn, typename... ___params>
				auto for_each(___fn&& ___fu, ___params&&... ___args)

					// check noexcept function
					noexcept(noexcept(___fu(
									cs::declval<reference>(),
									cs::declval<___params>()...))) -> void {

					// loop over data
					for (___node* ___it = _head; ___it != nullptr; ___it = ___it->_next)
						___fu(___it->_issue, cs::forward<___params>(___args)...);
				}

				/* const for each */
				template <typename ___fn, typename... ___params>
				auto for_each(___fn&& ___fu, ___params&&... ___args) const

					// check noexcept function
					noexcept(noexcept(___fu(
									cs::declval<const_reference>(),
									cs::declval<___params>()...))) -> void {

					// loop over data
					for (___node* ___it = _head; ___it != nullptr; ___it = ___it->_next)
						___fu(___it->_issue, cs::forward<___params>(___args)...);
				}


			private:

				// -- private methods -----------------------------------------

				/* init */
				auto _init(void) noexcept -> void;

				/* clear */
				auto _clear(void) noexcept -> void;

				/* copy */
				auto _copy(const ___self&) noexcept -> void;


		}; // class issues

	} // namespace core

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_CORE_ISSUES_HPP___
