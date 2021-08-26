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

Known Unicode Encodings
=======================

Out of all the encodings listed on the :doc:`encodings page </encodings>`, only a handful are known to be Unicode Encodings. These are as follows:

- UTF-7
- UTF-7-IMAP
- UTF-8
- UTF-16 (All Endiannesses)
- UTF-32 (All Endiannesses)
- GB18030
- CESU-8
- MUTF-8
- WTF-8
- UTF-1
- UTF-EBCDIC

When the encoding is known at compile time (e.g., it is just a plain object), it contains a ``is_unicode_encoding`` type member that is set to ``std::true_type``. Otherwise, it is left off. This is detected by :doc:`ztd::text::contains_unicode_encoding </api/contains_unicode_encoding>` and :doc:`ztd::text::is_unicode_encoding </api/is_unicode_encoding>`.

If you know of any others, please let us know!
