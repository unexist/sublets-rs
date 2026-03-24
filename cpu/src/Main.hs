--
-- @package sublets-rs
--
-- @file CPU plugin functions
-- @copyright (c) 2026-present Christoph Kappel <christoph@unexist.dev>
-- @version $Id$
--
-- This program can be distributed under the terms of the GNU GPLv3.
-- See the file LICENSE for details.
--

{-# LANGUAGE DeriveDataTypeable #-}

module Hello where

import Data.Maybe
import Extism.PDK
import Extism.PDK.JSON

defaultGreeting = "Hello"

greet g n =
  output $ g ++ ", " ++ n

testing = do
  -- Get a name from the Extism runtime
  name <- inputString
  -- Get  configured greeting
  greeting <- getConfig "greeting"
  -- Greet the user, if no greeting is configured then "Hello" is used
  greet (fromMaybe defaultGreeting greeting) name

foreign export ccall "greet" testing :: IO ()
