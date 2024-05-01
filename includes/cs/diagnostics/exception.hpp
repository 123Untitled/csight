#ifndef ___CS_EXCEPTION_HPP___
#define ___CS_EXCEPTION_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- E X C E P T I O N ---------------------------------------------------

	class exception {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::exception;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			exception(void) = default;

			/* copy constructor */
			exception(const ___self&) noexcept = default;

			/* move constructor */
			exception(___self&&) noexcept = default;

			/* destructor */
			virtual ~exception(void) = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public interface --------------------------------------------

			/* what */
			virtual auto what(void) const noexcept -> const char* = 0;

	}; // class exception


	// -- R U N T I M E  E R R O R --------------------------------------------

	class runtime_error : public cs::exception {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::runtime_error;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			runtime_error(void) = default;

			/* message constructor */
			runtime_error(const char*) noexcept;

			/* copy constructor */
			runtime_error(const ___self&) noexcept = default;

			/* move constructor */
			runtime_error(___self&&) noexcept = default;

			/* destructor */
			virtual ~runtime_error(void) = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public interface --------------------------------------------

			/* what */
			virtual auto what(void) const noexcept -> const char* override;


		private:

			// -- private members ---------------------------------------------

			/* message */
			const char* _message;

	}; // class runtime_error

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_EXCEPTION_HPP___
