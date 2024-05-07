#ifndef ___CS_DESCRIPTOR_HPP___
#define ___CS_DESCRIPTOR_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- U N I Q U E  D E S C R I P T O R ------------------------------------

	class descriptor {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::descriptor;


		public:

			// -- public constants --------------------------------------------

			/* invalid descriptor */
			enum : int { INVALID_DESCRIPTOR = -1 };


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			descriptor(void) noexcept;

			/* descriptor constructor */
			descriptor(const int) noexcept;

			/* deleted copy constructor */
			descriptor(const ___self&) = delete;

			/* move constructor */
			descriptor(___self&&) noexcept;

			/* destructor */
			~descriptor(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment */
			auto operator=(___self&&) noexcept -> ___self&;

			/* int assignment operator */
			auto operator=(const int) noexcept -> ___self&;


			// -- public accessors --------------------------------------------

			/* handle */
			auto handle(void) const noexcept -> int;


			// -- public conversion operators ---------------------------------

			/* int conversion operator */
			operator int(void) const noexcept;

			/* bool conversion operator */
			explicit operator bool(void) const noexcept;


			// -- public operators --------------------------------------------

			/* not operator */
			auto operator!(void) const noexcept -> bool;


			// -- public modifiers --------------------------------------------

			/* close */
			auto close(void) noexcept -> void;


		protected:



			// -- private members ---------------------------------------------

			/* descriptor */
			int _desc;


		private:

			// -- private methods ---------------------------------------------

			/* close */
			auto _close(void) noexcept -> void;

	}; // class descriptor

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_DESCRIPTOR_HPP___
