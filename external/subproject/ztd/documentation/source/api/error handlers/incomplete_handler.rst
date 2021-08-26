.. =============================================================================
..
.. ztd.text
.. Copyright © 2021 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
.. Contact: opensource@soasis.org
..
.. Commercial License Usage
.. Licensees holding valid commercial ztd.text licenses may use this file in
.. accordance with the commercial license agreement provided with the
.. Software or, alternatively, in accordance with the terms contained in
.. a written agreement between you and Shepherd's Oasis, LLC.
.. For licensing terms and conditions see your agreement. For
.. further information contact opensource@soasis.org.
..
.. Apache License Version 2 Usage
.. Alternatively, this file may be used under the terms of Apache License
.. Version 2.0 (the "License") for non-commercial use; you may not use this
.. file except in compliance with the License. You may obtain a copy of the
.. License at
..
.. 		https://www.apache.org/licenses/LICENSE-2.0
..
.. Unless required by applicable law or agreed to in writing, software
.. distributed under the License is distributed on an "AS IS" BASIS,
.. WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
.. See the License for the specific language governing permissions and
.. limitations under the License.
..
.. =============================================================================>

incomplete_handler
==================

This error handler takes the ``ztd::text::encoding_error::incomplete_sequence`` error and uses it to read from the provided "progress" contiguous range provided as the third parameter to any error handler. This can be helpful in situations here incomplete input is not to be interpreted as an error, such as in situations with networking stacks, I/O reads (particularly, non-recoverable streams like ``std::istream_iterator``\ s or ``std::ostream_iterator``\ s), and other such storage cases.

The data read but not used from an incomplete error during encode and decode operations is stored in the ``incomplete_handler`` object and can be accessed VIA the ``code_points`` and ``code_units`` functions.

.. doxygenclass:: ztd::text::incomplete_handler
	:members:
