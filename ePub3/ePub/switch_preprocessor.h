//
//  switch_preprocessor.h
//  ePub3
//
//  Created by Jim Dovey on 2013-01-31.
//  Copyright (c) 2012-2013 The Readium Foundation.
//
//  The Readium SDK is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef __ePub3__switch_preprocessor__
#define __ePub3__switch_preprocessor__

#include "epub3.h"
#include "filter.h"
#include <vector>
#include <regex>

EPUB3_BEGIN_NAMESPACE

/**
 The SwitchPreprocessor will locate any <epub:switch> compounds in an EPUB Content
 Document and will statically alter their contents by replacing each switch compound
 with the content of an epub:case element or epub:default element depending on the
 namespaces supported.
 
 Documents are identified by checking their manifest items for the `switch` property.
 If a document contains an epub:switch statement but doesn't have this property,
 then that file will be passed through unchanged.
 
 It should be used only for reading, never for writing.
 */
class SwitchPreprocessor : public ContentFilter
{
public:
    ///
    /// A list of supported namespaces, as strings.
    typedef std::vector<string>     NamespaceList;
    
protected:
    /// Only documents whose manifest items are XHTML with the `switch` property
    /// will be filtered.
    static bool SniffSwitchableContent(const ManifestItem* item, const EncryptionInfo* encInfo);
    
public:
    /**
     This constructor creates a preprocessor which supports content identified by
     any of the provided namespaces.
     @param supportedNamespaces A list of namespaces whose content is supported by
     the renderer.
     */
    SwitchPreprocessor(const NamespaceList& supportedNamespaces) : ContentFilter(SniffSwitchableContent), _supportedNamespaces(supportedNamespaces) {}
    
    /**
     The default constructor indicates that no additional content is supported, and
     the resulting filter will only preserve the content of epub:default tags.
     */
    SwitchPreprocessor() : ContentFilter(SniffSwitchableContent), _supportedNamespaces() {}
    
    ///
    /// The standard copy constructor.
    SwitchPreprocessor(const SwitchPreprocessor& o) : ContentFilter(o), _supportedNamespaces(o._supportedNamespaces) {}
    
    ///
    /// The standard C++11 'move' constructor.
    SwitchPreprocessor(SwitchPreprocessor&& o) : ContentFilter(std::move(o)), _supportedNamespaces(std::move(o._supportedNamespaces)) {}
    
    /**
     This processor won't work on streamed data, it requires the whole thing at once.
     */
    virtual bool RequiresCompleteData() const { return true; }
    
    /**
     Filters the input data using regular expressions to identify epub:switch
     compounds and replace them wholesale wih the contents of an epub:case or
     epub:default element.
     
     If the list of supported namespaces is empty, then this takes an optimized path,
     ignoring epub:case elements completely. Otherwise, it will inspect the 
     `required-namespace` attribute of each case element and will match it against
     the contents of its supported namespace list to make a decision. The first
     matching epub:case statement will be output in place of the entire switch
     compound.
     */
    virtual void * FilterData(void *data, size_t len, size_t *outputLen);
    
protected:
    ///
    /// All the namespaces for content to be allowed through the filter.
    NamespaceList   _supportedNamespaces;
    
private:
    /**
     A regex to un-comment any epub:switch blocks.
     
     For instance, we might see:
     
         <!--<epub:switch id="bob">
           <epub:case required-namespace="...">
              ...
           </epub:case>
           <epub:default>-->
             <img src="..." /><!--
           </epub:default>
         </epub:switch>-->
     
     This regex will identify such constructs, but will NOT match an entire
     commented-out epub:switch block (i.e. where the publisher has chosen to comment
     out the whole thing and provide only the default content).
     */
    static std::regex   CommentedSwitchIdentifier;
    
    /**
     This expression locates and isolates the content of each epub:switch element.
     */
    static std::regex   SwitchContentExtractor;
    
    /**
     This expression locates and extracts the contents and types of each epub:case
     element.
     */
    static std::regex   CaseContentExtractor;
    
    /**
     This expression locates and extracts the content of the epub:default element.
     */
    static std::regex   DefaultContentExtractor;
    
};

EPUB3_END_NAMESPACE

#endif /* defined(__ePub3__switch_preprocessor__) */
