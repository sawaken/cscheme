# -*- mode: ruby -*-

C_COMPILER = "gcc -std=c99"
SRC_DIR = "./src"

TYPES = [:bool, :cell, :character, :continuation, :dummy, :env, :exception, :form,
         :integer, :lambda, :meta_object, :parameter, :prim_func, :special_form,
         :string, :symbol] + [:type]

SRCS = [:main, :special_forms, :prim_funcs, :cscheme, :eval, :parse, :parse_util, :util]

OBJECTS = SRCS.map{|s| "#{SRC_DIR}/#{s}.o"} + TYPES.map{|t| "#{SRC_DIR}/type/#{t}.o"}

task :default => "cscheme"

task :clean do
  puts "---- clean ----"
  sh "rm -f cscheme"
  sh "rm -f ./src/*.o ./src/type/*.o"
end

rule ".o" => ".c" do |t|
  sh "#{C_COMPILER} -c #{t.source} -o #{t.name}"
end

file "cscheme" => OBJECTS do |t|
  sh "#{C_COMPILER} -o #{t.name} #{t.prerequisites.join(' ')}"
end


