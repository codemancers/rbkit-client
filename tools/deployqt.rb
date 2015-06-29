#!/usr/bin/env ruby

require "fileutils"

class DeployQt
  attr_accessor :qt_home, :executable, :dst
  def initialize(executable)
    @pwd = ENV["PWD"]
    @executable = File.expand_path(File.join(@pwd, executable))
    @qt_home = "#{ENV['QT_HOME']}/5.4/#{compiler_version}"
    @dst = File.expand_path(File.join(@pwd, "/rbkit"))
    remove_existing_files
    @platform_plugin = File.expand_path(File.join(@qt_home, "/plugins/platforms/libqxcb.so"))
    @sql_driver_path = File.expand_path(File.join(@qt_home, "/plugins/sqldrivers/libqsqlite.so"))
    FileUtils.mkdir_p(@dst)
    make_required_dirs
  end

  def make_required_dirs
    FileUtils.mkdir_p("#{dst}/fonts")
    FileUtils.mkdir_p("#{dst}/libs")
    FileUtils.mkdir_p("#{dst}/platforms")
    FileUtils.mkdir_p("#{dst}/sqldrivers")
  end

  def remove_existing_files
    FileUtils.rm_rf(dst)
    FileUtils.rm_rf("rbkit-#{arch}.tar.gz")
  end

  def create_archive
    copy_app_libs
    copy_lib_dependencies
    copy_sql_dependencies
    make_executable
    create_tar_file
  end

  def create_tar_file
    FileUtils.cd(@pwd) do
      system("tar -zcvf rbkit-#{arch}.tar.gz rbkit")
    end
  end

  def copy_sql_dependencies
    FileUtils.cp(@sql_driver_path, "#{dst}/sqldrivers")
  end

  def copy_app_libs
    puts "Copying application dependencies for #{executable}"
    `ldd #{executable}`.tap do |deps|
      deps_array = deps.split("\n")
      deps_array.each { |deps_element| verify_and_copy(deps_element) }
    end
  end

  def copy_lib_dependencies
    FileUtils.cp(@platform_plugin, "#{dst}/platforms")
    `ldd #{@platform_plugin}`.tap do |deps|
      deps_array = deps.split("\n")
      deps_array.each { |deps_element| verify_and_copy(deps_element) }
    end
  end

  def make_executable
    exec_string =<<-EOD
#!/bin/sh
export LD_LIBRARY_PATH=\`pwd\`/libs
export QT_QPA_FONTDIR=\`pwd\`/fonts

cd "$(dirname "$0")"
./#{File.basename(executable)}
    EOD
    FileUtils.cp(executable, dst)
    File.open("#{dst}/rbkit", "w") do |fl|
      fl.puts(exec_string)
    end
    system("chmod +x #{dst}/rbkit")
  end

  private

  def verify_and_copy(deps_element)
    dep_path = deps_element.split("=>").last.split("(").first.strip
    if !dep_path.empty? && dep_path.match(/^#{ENV['HOME']}/)
      FileUtils.cp(File.expand_path(dep_path), "#{dst}/libs")
    end
  end

  def compiler_version
    arch == 'i686' ? 'gcc' : 'gcc_64'
  end

  def arch
    `uname -m`.strip
  end
end

DeployQt.new(ARGV[0]).create_archive
