import { Feature } from "./feature";
export type CompgenAction = 'alias' | 'arrayvar' | 'binding' | 'builtin' | 'command' | 'directory' | 'disabled' | 'enabled' | 'export' | 'file' | 'function' | 'group' | 'helptopic' | 'job' | 'keyword' | 'running' | 'service' | 'setopt' | 'shopt' | 'signal' | 'stopped' | 'user' | 'variableq';
export declare function Compgen(action: CompgenAction, word?: string, cwd?: string): string[];
declare class DefaultCompleteFeature extends Feature {
    brief: string;
    description: string;
    debug: boolean;
    entry(): Promise<number>;
}
export declare const defaultCompleteFeature: DefaultCompleteFeature;
export {};
